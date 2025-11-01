# ESP32 Light Sensor Pump Control

I’m currently working on a project inspired by systems like the **Hach CL17**, for my own testing and experimentation with IoT automation.  

This first version uses a **single on/off digital light sensor** to control a “pump” LED and includes **Wi-Fi control** for the built-in LED for testing purposes. The next step will be to make the system more automated with simulated chemical feed logic.

## Features
- Wi-Fi toggle for built-in LED (GPIO 2)  
- Light sensor-controlled “pump” LED (GPIO 4)  
- Serial Monitor output for live sensor and pump feedback  
- Simple web interface for testing and control

## Hardware
| Component | Pin | Notes |
|-----------|-----|------|
| Built-in LED | GPIO 2 | Web-controlled LED |
| Pump LED | GPIO 4 | Turns on when sensor detects “dark” |
| Light Sensor DO | GPIO 15 | Digital signal from sensor |
| Light Sensor VCC | 3.3V | Power |
| Light Sensor GND | GND | Common ground |

## Wiring Summary
ESP32 3.3V ----> Sensor VCC
ESP32 GND ----> Sensor GND
ESP32 GPIO15 --> Sensor DO
ESP32 GPIO4 ---> 220Ω ---> Pump LED (+)
ESP32 GND ---> Pump LED (-)


## Next Steps
- Expand automation to simulate chemical dosing logic  
- Add more sensors and actuators  
- Enhance web interface to display sensor states in real-time


