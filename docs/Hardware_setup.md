# Smart Helmet – Hardware Notes

## Power Tips
- ESP32 uses 3.3V
- GPS works on 5V
- SIM800L **must use 4V @ 2A**
  - Use buck converter **LM2596** or **AMS1117 + capacitor bank**
- WS2812B needs clean 5V & common ground

## GPS Placement
- Mount near helmet top or visor
- Avoid metal shielding over antenna

## SIM800L SIM Tips
- Use 4G SIM — supports 2G fallback
- Disable PIN lock before inserting SIM
