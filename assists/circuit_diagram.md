# Smart Helmet Circuit Wiring (ESP32)

## ESP32 — Core Connections

### MPU6050 (I2C)
MPU6050 VCC  → ESP32 3.3V  
MPU6050 GND  → ESP32 GND  
MPU6050 SDA  → GPIO21  
MPU6050 SCL  → GPIO22  

### GPS NEO-6M
GPS VCC → 5V  
GPS GND → GND  
GPS TX  → GPIO17 (RX1)  
GPS RX  → GPIO16 (TX1)  

### SIM800L
Requires 4V DC @ 2A  
SIM800L VCC → Step-down Buck 4.0V  
SIM800L GND → GND (shared)  
SIM800L TX → GPIO27  
SIM800L RX → GPIO26  
SIM800L EN → optional ON/OFF pin  

### WS2812B LED Strip
LED VCC → 5V  
LED GND → GND  
LED DIN → GPIO27  
(Use 330Ω resistor inline recommended)

### Buzzer
BUZZER + → GPIO33  
BUZZER – → GND

### SOS Tact Switch
One pin → GPIO32  
Other pin → GND  
(Internal pull-up enabled)

### Status LED (Optional onboard)
STATUS LED + → GPIO25  
STATUS LED – → GND

---

## Common Ground Rule
All modules **share a single common GND**:
- ESP32
- SIM800L
- GPS
- MPU6050
- WS2812B
- Buzzer
- Button

---

## Suggested Extras
- 470µF capacitor across LED VCC–GND
- 1000µF capacitor on SIM800L VCC–GND
- Heat shrink + hot glue inside helmet
