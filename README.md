**# ESP32_Hybrid-IoT-System_-Smart-Street-Light-_BLYNK-IoT-Cloud**

**Demonstration:** 

The ESP32-based Smart Street Light performs local edge decision-making using an LDR sensor and controls the 26-LED lamp through a relay. The system is connected to Blynk Cloud for monitoring and provides Auto/Manual operating modes. In Auto mode, the ESP32 locally determines the lighting condition and controls the lamp; in Manual mode, the lamp can be controlled remotely through Blynk.

⚙️ WORKING / OPERATION OF THE PROJECT

The ESP32-Based Smart Street Light uses an LDR sensor to continuously monitor the surrounding light intensity. The sensor value is read locally by the ESP32, where the programmed edge decision logic determines whether the environment is DARK or BRIGHT.

🌙 DARK condition: If the LDR value falls below the predefined threshold, the ESP32 makes a local decision and activates the relay, turning ON the 26-LED street-light lamp.

☀️ BRIGHT condition: When the LDR value rises above the light threshold, the ESP32 locally decides that sufficient light is available and deactivates the relay, turning OFF the LED lamp.

📱 Blynk Cloud Monitoring: The ESP32 sends the LDR reading to Blynk Cloud through Wi-Fi, allowing the sensor value and system operation to be monitored remotely.

🔄 Automatic Mode: In AUTO mode, the lamp is controlled automatically according to the LDR sensor and the decision logic running locally on the ESP32.

🕹️ Manual Mode: In MANUAL mode, the automatic decision is overridden and the user can remotely turn the lamp ON/OFF through the Blynk application.

⚡ Hybrid Operation: Thus, the project combines local Edge Processing + IoT Cloud Monitoring + Manual Remote Control, providing a responsive and semi-intelligent smart lighting system.

**🔁 Overall Operation**

LDR Sensor → ESP32 Edge Processing → Decision Logic → Relay → 26-LED Lamp

ESP32 ↔ Wi-Fi ↔ Blynk Cloud → Monitoring / Manual Override

**Live Demo Video 👇👇**

https://youtu.be/ZFBkcDMe2nc?si=Eg5Db_IkmxV2wtVp
