# Smart Helmet – Accident Detection, SOS & Navigation (ESP32)

A safety-focused Smart Helmet using **ESP32**, **MPU6050**, **GPS NEO-6M**, **SIM800L**, and **WS2812B RGB LEDs**.

Features real-time tilt-based indications, crash alerts with GPS
coordinates, SOS button-triggered help messages, and over-speed alarms.

---

## Features

### Crash Detection & SOS
- Detects sudden acceleration spike using MPU6050
- Automatically sends SMS with **Google Maps link**
- Manual triple-button-click SOS option

### Live GPS + Speed Alerts
- Reads coordinates from NEO-6M
- Calculates speed
- Sends SMS when speed crosses limit

### Auto Turn Indicators via Tilt
- Lean right → Right LED panel glows
- Lean left → Left panel glows
- Straight posture turns lights OFF

### LED Helmet Animation
- 5-second startup pulse animation using WS2812B

---

## Hardware Components

| Component | Purpose |
|----------|----------|
| ESP32 Dev Kit | Main controller |
| MPU6050 | Crash + tilt detection |
| NEO-6M GPS | Location + speed |
| SIM800L | SMS messaging |
| WS2812B LED strip (60 LEDs) | Indicators + animations |
| Buzzer | Alerts |
| Push Button (SOS) | Triple click → emergency |
| Helmet Shell | Assembly |

---

## Wiring / Connections (ESP32)

### **MPU6050**
```
VCC → 3.3V/5V
GND → GND
SDA → GPIO21
SCL → GPIO22
```

### **GPS NEO-6M**
```
VCC → 5V
GND → GND
TX  → GPIO17 (ESP RX)
RX  → GPIO16 (ESP TX)
```

### **SIM800L**
Needs **4V & 2A** power (not 5V!)
```
VCC → 4V regulator output
GND → Common Ground
TX  → GPIO27 (ESP RX)
RX  → GPIO26 (ESP TX)
```

### **WS2812B LEDs**
```
DIN → GPIO27
VCC → 5V
GND → Common Ground
```

### **Others**
```
Buzzer       → GPIO33
Status LED   → GPIO25
SOS Button   → GPIO32 (PULLUP)
```

---

## Folder Structure

```
smart-helmet/
│
├── arduino-code/
│   └── smart_helmet.ino
│
├── docs/
│   ├── hardware-setup.md
│   └── future-improvements.md
│
├── assets/
│   └── circuit-diagram.png
├── .gitignore
└── README.md
```

---

## Flashing Instructions
1. Open Arduino IDE 2.x
2. Install board **ESP32 Dev Module**
3. Install required libraries:
   - MPU6050
   - TinyGPS++
   - Adafruit NeoPixel
4. Select port → Upload

---

## Threshold Tuning
Edit inside main sketch:
```cpp
#define SPEED_LIMIT        1.0   // km/h
#define TILT_THRESHOLD     15000
#define TILT_HYSTERESIS    3000
```

---

## Contribute
Spotted a bug or want to add Bluetooth/app support?
Feel free to create a Pull Request

---

## Team

**Kandula Sai Gana Laxmi Rohith** 

**ANCHETTI DEEKSHITH** 

**GORUGANTI SAI KHOWSHIK** 

📍 Hyderabad, India
