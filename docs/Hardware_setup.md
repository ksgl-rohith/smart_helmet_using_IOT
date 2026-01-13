# Hardware & Wiring Guide

## 1️⃣ Power
- Arduino + MPU6050 + GPS → 5V
- SIM800L needs stable **4V 2A** (use buck converter)
- WS2812B LEDs → 5V + common GND

## 2️⃣ Connections
MPU6050  
- VCC → 5V  
- GND → GND  
- SDA → A4  
- SCL → A5

GPS NEO-6M  
- VCC → 5V  
- TX → D4  
- RX → D3  

SIM800L  
- VCC → 4V regulated  
- TX → D8  
- RX → D7  
- GND → Common

WS2812B  
- DIN → D6  
- VCC → 5V  
- GND → Common

Buzzer → D9
