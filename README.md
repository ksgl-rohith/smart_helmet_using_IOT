# Smart Helmet – Accident Detection, Alert & Navigation

A safety-focused Smart Helmet built using **Arduino**, **MPU6050**, **GPS**, **SIM800L**, and **WS2812B LEDs**.  
It detects vehicle accidents, sends emergency alerts via SMS, warns about over-speeding, and provides automatic turn indication based on head tilt.  
Also features a 4-second LED startup animation.

---

## Features

### Accident Detection
- Detects sudden impact/jerk using **MPU6050**
- Sends SMS with location to emergency contact
- Auto-trigger SMS if rider loses consciousness

### Live Location Tracking (GPS)
- Reads latitude & longitude from **NEO-6M GPS**
- Shares location via SIM800L when crash detected

### Overspeed Warning
- Speed calculated from GPS coordinates
- Triggers buzzer + LED warning over threshold

### Head-Tilt based Indicators
- Lean **left/right** = Turn indicators ON
- Comes back upright = OFF

### LED Boot Animation
- WS2812B LEDs display attractive pattern for 4 seconds on power-up

---

## Hardware Components
| Component | Purpose |
|----------|----------|
| Arduino Uno/Nano | Main controller |
| MPU6050 | Tilt + acceleration sensor |
| NEO-6M GPS | Speed + coordinates |
| SIM800L/SIM900A | SMS alerts |
| Relay Module | Ignition control (optional) |
| WS2812B LED strip | Indicators & animation |
| Buzzer | Alerts |
| Helmet | Fitting all electronics |

---

## Folder Structure

```
smart-helmet/
│
├── /arduino-code
│   └── smart_helmet.ino
│
├── /assets
│   ├── circuit-diagram.png
│   └── demo-video.gif 
│
├── /docs
│   ├── hardware-setup.md
│   └── future-improvements.md
├── .gitignore
└── README.md
```

---

## Arduino Libraries Used
- `Wire.h`
- `Adafruit_MPU6050.h`
- `Adafruit_Sensor.h`
- `TinyGPS++.h`
- `SoftwareSerial.h`
- `Adafruit_NeoPixel.h`

Install from **Arduino Library Manager**.

---

## How it Works (Flow)
1. System boots → LED startup animation plays  
2. Continuously reads:
   - Tilt angle
   - Acceleration (crash)
   - GPS speed + location
3. If crash detected → Send SMS to emergency contact  
4. If speed > limit → Sound buzzer + flash LEDs  
5. Detect head tilt → Activate left/right indicators  
6. Reset automatically when bike upright

---

## Testing & Calibration
- Modify threshold values inside code:
```cpp
float crashSensitivity = 3.5;  // Adjust based on tests
int speedLimit = 60;           // km/h
float tiltThreshold = 18;      // degrees
```

---

## Future Improvements
- Add fall detection with timer cancel button
- Bluetooth mobile app dashboard
- Helmet strap sensor (disable engine until strapped)
- IoT MQTT integration

---

## Contributions
Pull requests, ideas and improvements are welcome!

---

## Team

**Kandula Sai Gana Laxmi Rohith** 

**ANCHETTI DEEKSHITH** 

**GORUGANTI SAI KHOWSHIK** 

📍 Hyderabad, India
