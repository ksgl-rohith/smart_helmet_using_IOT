#include <Wire.h>
#include <MPU6050.h>
#include <TinyGPS++.h>
#include <Adafruit_NeoPixel.h>

// -------------------- PIN DEFINITIONS --------------------
#define MPU_SDA       21
#define MPU_SCL       22
#define GPS_RX        16
#define GPS_TX        17
#define SIM800_TX     26
#define SIM800_RX     27
#define BUZZER_PIN    33
#define STATUS_LED    25
#define LED_PIN       27
#define BUTTON_PIN    32 // Tact switch

// -------------------- CONSTANTS --------------------------
#define SPEED_LIMIT   1.0   // km/h
#define NUM_LEDS      60
#define NUM_ROWS      5
#define NUM_COLS      12
#define SMS_DELAY     1000
#define TILT_THRESHOLD 15000
#define TILT_HYSTERESIS 3000
#define BUTTON_PRESS_INTERVAL 500 // ms
#define BUTTON_PRESS_COUNT    3

// -------------------- OBJECT DECLARATIONS ----------------
MPU6050 mpu;
TinyGPSPlus gps;
HardwareSerial sim800(2);   // SIM800L on UART2
HardwareSerial GPS_Serial(1);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// -------------------- STATE VARIABLES --------------------
bool leftIndicatorOn = false;
bool rightIndicatorOn = false;
unsigned long lastButtonPressTime = 0;
int buttonPressCounter = 0;

// -------------------- EMERGENCY CONTACTS -----------------
const char* emergencyContacts[] = {
  "+918885414469",
  "+918143570858"
};
const int contactCount = sizeof(emergencyContacts) / sizeof(emergencyContacts[0]);

// -------------------- HELPER: LED Mapping ----------------
int getLEDIndex(int row, int col) {
  if (row % 2 == 0) {
    return row * NUM_COLS + col;
  } else {
    return row * NUM_COLS + (NUM_COLS - 1 - col);
  }
}

// -------------------- HELPER: Color Wheel ----------------
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// -------------------- STARTUP ANIMATION ------------------
void startupAnimation() {
  strip.clear();
  for (int radius = 0; radius < max(NUM_ROWS, NUM_COLS); radius++) {
    for (int row = 0; row < NUM_ROWS; row++) {
      for (int col = 0; col < NUM_COLS; col++) {
        int centerRow = NUM_ROWS / 2;
        int centerCol = NUM_COLS / 2;
        int dist = abs(row - centerRow) + abs(col - centerCol);
        if (dist == radius) {
          int ledIndex = getLEDIndex(row, col);
          strip.setPixelColor(ledIndex, strip.Color(255, 165, 0)); // Cyan mirage
        }
      }
    }
    strip.show();
    delay(100);
  }
  delay(5000);
  strip.clear();
  strip.show();
}

// -------------------- DIRECTIONAL LIGHTS -----------------
void setLeftIndicator() {
  strip.clear();
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = 0; col < NUM_COLS / 2; col++) {
      int ledIndex = getLEDIndex(row, col);
      strip.setPixelColor(ledIndex, Wheel((col * 256 / (NUM_COLS / 2)) & 255));
    }
  }
  strip.show();
}

void setRightIndicator() {
  strip.clear();
  for (int row = 0; row < NUM_ROWS; row++) {
    for (int col = NUM_COLS / 2; col < NUM_COLS; col++) {
      int ledIndex = getLEDIndex(row, col);
      strip.setPixelColor(ledIndex, Wheel(((col - NUM_COLS / 2) * 256 / (NUM_COLS / 2)) & 255));
    }
  }
  strip.show();
}

// -------------------- DETECT TILT ------------------------
void detectTiltDirection() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);

  if (gy > TILT_THRESHOLD && !rightIndicatorOn) {
    rightIndicatorOn = true;
    leftIndicatorOn = false;
    setRightIndicator();
  } else if (gy < -TILT_THRESHOLD && !leftIndicatorOn) {
    leftIndicatorOn = true;
    rightIndicatorOn = false;
    setLeftIndicator();
  } else if (abs(gy) < (TILT_THRESHOLD - TILT_HYSTERESIS)) {
    if (leftIndicatorOn || rightIndicatorOn) {
      leftIndicatorOn = false;
      rightIndicatorOn = false;
      strip.clear();
      strip.show();
    }
  }
}

// -------------------- CRASH DETECTION --------------------
void detectCrash() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float totalAcc = sqrt(ax * ax + ay * ay + az * az) / 16384.0;

  if (totalAcc > 3.0 && gps.location.isValid()) {
    digitalWrite(BUZZER_PIN, HIGH);
    sendCrashAlert();
    delay(3000);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// -------------------- GPS SPEED MONITOR ------------------
void readGPSAndSpeed() {
  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }

  if (gps.location.isUpdated()) {
    float speed = gps.speed.kmph();
    if (speed > SPEED_LIMIT && gps.location.isValid()) {
      digitalWrite(BUZZER_PIN, HIGH);
      sendOverSpeedAlert(speed, gps.location.lat(), gps.location.lng());
      delay(3000);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

// -------------------- GSM ALERTS -------------------------
void sendCrashAlert() {
  for (int i = 0; i < contactCount; i++) {
    sim800.println("AT+CMGF=1");
    delay(500);
    sim800.print("AT+CMGS=\"");
    sim800.print(emergencyContacts[i]);
    sim800.println("\"");
    delay(500);
    sim800.print("🚨 Crash detected! Location: https://maps.google.com/?q=");
    sim800.print(gps.location.lat(), 6);
    sim800.print(",");
    sim800.print(gps.location.lng(), 6);
    sim800.write(26);
    delay(SMS_DELAY);
  }
}

void sendOverSpeedAlert(float speed, float lat, float lng) {
  for (int i = 0; i < contactCount; i++) {
    sim800.println("AT+CMGF=1");
    delay(500);
    sim800.print("AT+CMGS=\"");
    sim800.print(emergencyContacts[i]);
    sim800.println("\"");
    delay(500);
    sim800.print("⚠ Over-speeding! Speed: ");
    sim800.print(speed);
    sim800.print(" km/h. Location: https://maps.google.com/?q=");
    sim800.print(lat, 6);
    sim800.print(",");
    sim800.print(lng, 6);
    sim800.write(26);
    delay(SMS_DELAY);
  }
}

// -------------------- BUTTON SOS FEATURE -----------------
void checkSOSButton() {
  if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
    unsigned long now = millis();
    if (now - lastButtonPressTime < BUTTON_PRESS_INTERVAL) {
      buttonPressCounter++;
      if (buttonPressCounter >= BUTTON_PRESS_COUNT) {
        sendCrashAlert();
        buttonPressCounter = 0;
      }
    } else {
      buttonPressCounter = 1;
    }
    lastButtonPressTime = now;
    delay(200); // debounce
  }
}

// -------------------- SETUP ------------------------------
void setup() {
  Serial.begin(115200);
  Wire.begin(MPU_SDA, MPU_SCL);
  mpu.initialize();
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  sim800.begin(9600, SERIAL_8N1, SIM800_RX, SIM800_TX);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(STATUS_LED, HIGH);

  strip.begin();
  strip.show();
  startupAnimation();

  sim800.println("AT");
  delay(1000);
}

// -------------------- LOOP -------------------------------
void loop() {
  detectTiltDirection();
  readGPSAndSpeed();
  detectCrash();
  checkSOSButton();
}
