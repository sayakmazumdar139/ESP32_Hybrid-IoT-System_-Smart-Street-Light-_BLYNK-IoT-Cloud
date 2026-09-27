**# ESP32_Hybrid-IoT-System_-Smart-Street-Light-_BLYNK-IoT-Cloud**

**DEMONSTRATION:** 

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

https://youtu.be/aqz-LQ7ug8c




**⚙️ Task-06 Firmware — Step-by-Step Working / Operation**

**1️⃣ Blynk Configuration**

```cpp
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
```

→ Defines the **Blynk Template ID** of the Task-06 project.

```cpp
#define BLYNK_TEMPLATE_NAME "Task06 Smart Street Light"
```

→ Defines the **Blynk Template Name**.

```cpp
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"
```

→ Provides the ESP32's **Blynk authentication token** for cloud connection.

---

### 2️⃣ Required Libraries

```cpp
#include <WiFi.h>
```

→ Enables **Wi-Fi communication** using the ESP32.

```cpp
#include <BlynkSimpleEsp32.h>
```

→ Enables communication between the **ESP32 and Blynk Cloud**.

---

### 3️⃣ Wi-Fi Credentials

```cpp
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";
```

→ Stores the Wi-Fi network name and password used by the ESP32.

---

### 4️⃣ Hardware Pin Definition

```cpp
#define LDR_PIN 34
```

→ Connects the **LDR sensor output to GPIO34**.

```cpp
#define RELAY_PIN 26
```

→ Connects the **relay control input to GPIO26**.

---

### 5️⃣ Relay Logic

```cpp
#define RELAY_ON LOW
#define RELAY_OFF HIGH
```

→ Defines the relay as **active LOW**: LOW = ON and HIGH = OFF.

---

### 6️⃣ Edge-Decision Thresholds

```cpp
const int DARK_THRESHOLD = 1600;
```

→ Below **1600**, the ESP32 considers the environment **DARK**.

```cpp
const int LIGHT_THRESHOLD = 2000;
```

→ Above **2000**, the ESP32 considers the environment **BRIGHT**.

The gap between 1600 and 2000 provides **hysteresis**, helping prevent rapid switching near the threshold.

---

### 7️⃣ Operating Variables

```cpp
bool autoMode = true;
```

→ Starts the system in **AUTO mode**.

```cpp
bool manualLampState = false;
```

→ Stores the requested manual lamp state.

```cpp
bool lampState = false;
```

→ Stores the lamp's current ON/OFF state.

```cpp
int ldrValue = 0;
```

→ Stores the latest LDR sensor reading.

```cpp
const unsigned long SENSOR_INTERVAL = 500;
```

→ Sets the sensor-processing interval to **500 ms**.

```cpp
BlynkTimer timer;
```

→ Creates a timer for periodic sensor processing.

---

# 8️⃣ `setLamp()` — Relay/Lamp Control

```cpp
void setLamp(bool state)
```

→ Function used to turn the lamp **ON or OFF**.

```cpp
lampState = state;
```

→ Saves the current lamp state.

```cpp
digitalWrite(RELAY_PIN, state ? RELAY_ON : RELAY_OFF);
```

→ Sends the appropriate signal to GPIO26 and controls the relay.

```cpp
Serial.print("Lamp: ");
Serial.println(state ? "ON" : "OFF");
```

→ Displays the lamp status in the **Serial Monitor**.

---

# 9️⃣ `automaticDecision()` — Edge Intelligence

```cpp
void automaticDecision()
```

→ Performs the **local decision-making** on the ESP32.

```cpp
if (!lampState)
```

→ Checks whether the lamp is currently OFF.

```cpp
if (ldrValue < DARK_THRESHOLD)
```

→ If the LDR value is below 1600, darkness is detected.

```cpp
setLamp(true);
```

→ Turns the lamp **ON** through the relay.

```cpp
Serial.println("EDGE DECISION: DARK -> Lamp ON");
```

→ Prints the local edge decision.

---

### When the lamp is already ON:

```cpp
else
```

→ Checks the condition for turning the lamp OFF.

```cpp
if (ldrValue > LIGHT_THRESHOLD)
```

→ If LDR value becomes greater than 2000, bright light is detected.

```cpp
setLamp(false);
```

→ Turns the lamp **OFF**.

```cpp
Serial.println("EDGE DECISION: BRIGHT -> Lamp OFF");
```

→ Displays the edge decision in Serial Monitor.

---

# 🔟 `readSensorAndProcess()` — Main Sensor Processing

```cpp
void readSensorAndProcess()
```

→ Periodically reads the sensor and processes the decision.

```cpp
ldrValue = analogRead(LDR_PIN);
```

→ Reads the LDR's analog value through **GPIO34**.

```cpp
Serial.print("LDR Value: ");
Serial.println(ldrValue);
```

→ Displays the current LDR value.

```cpp
if (autoMode) automaticDecision();
```

→ If AUTO mode is selected, the ESP32 performs the local decision.

```cpp
Blynk.virtualWrite(V2, ldrValue);
```

→ Sends the LDR value to **Blynk V2** for remote monitoring.

---

### Blynk Status

```cpp
if (autoMode)
```

→ Checks whether the system is operating automatically.

```cpp
Blynk.virtualWrite(V3, ...);
```

→ Sends the current system status to the **V3 String datastream**.

Examples:

* `AUTO - DARK - LAMP ON`
* `AUTO - BRIGHT - LAMP OFF`
* `MANUAL - LAMP ON`
* `MANUAL - LAMP OFF`

---

# 1️⃣1️⃣ V0 — AUTO/MANUAL Mode Control

```cpp
BLYNK_WRITE(V0)
```

→ Executes whenever the **V0 Blynk switch** changes.

```cpp
int value = param.asInt();
```

→ Reads the switch value.

```cpp
if (value == 1)
```

→ V0 = 1 means **MANUAL mode**.

```cpp
autoMode = false;
```

→ Disables automatic decision-making.

```cpp
setLamp(manualLampState);
```

→ Sets the lamp according to the manual state.

---

```cpp
else
```

→ V0 = 0 means **AUTO mode**.

```cpp
autoMode = true;
```

→ Enables automatic edge processing.

```cpp
automaticDecision();
```

→ Immediately evaluates the current LDR condition.

---

# 1️⃣2️⃣ V1 — Manual Lamp Control

```cpp
BLYNK_WRITE(V1)
```

→ Executes when the **Manual Lamp switch** changes.

```cpp
manualLampState = param.asInt();
```

→ Reads the requested ON/OFF state from Blynk.

```cpp
if (!autoMode)
```

→ Checks whether the system is currently in MANUAL mode.

```cpp
setLamp(manualLampState);
```

→ Turns the lamp ON/OFF according to the user's Blynk command.

```cpp
else
```

→ If AUTO mode is active...

```cpp
Serial.println("Ignored: System is currently in AUTO mode.");
```

→ The manual command is ignored because the ESP32 is controlling the lamp automatically.

---

# 1️⃣3️⃣ `setup()` — Initial Startup

```cpp
void setup()
```

→ Runs **once** when the ESP32 starts.

```cpp
Serial.begin(115200);
```

→ Starts Serial Monitor communication at **115200 baud**.

```cpp
pinMode(RELAY_PIN, OUTPUT);
```

→ Configures GPIO26 as an output.

```cpp
digitalWrite(RELAY_PIN, RELAY_OFF);
```

→ Keeps the relay/lamp OFF during startup.

```cpp
analogReadResolution(12);
```

→ Sets ESP32 ADC resolution to **12-bit**, giving readings from **0–4095**.

```cpp
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
```

→ Connects the ESP32 to **Wi-Fi and Blynk Cloud**.

```cpp
timer.setInterval(SENSOR_INTERVAL, readSensorAndProcess);
```

→ Calls the sensor-processing function every **500 ms**.

```cpp
Serial.println("System ready.");
```

→ Indicates that the system has started.

---

# 1️⃣4️⃣ `loop()` — Continuous Operation

```cpp
void loop()
```

→ Runs continuously after `setup()`.

```cpp
Blynk.run();
```

→ Maintains communication between **ESP32 and Blynk Cloud**.

```cpp
timer.run();
```

→ Executes the scheduled sensor-reading and decision function.

---

# 🔄 COMPLETE FIRMWARE OPERATION

```text
             LDR Sensor
                 ↓
          GPIO34 / ADC
                 ↓
          ESP32 reads value
                 ↓
        ┌─────────────────┐
        │ Edge Decision    │
        │ Logic            │
        └─────────────────┘
           ↓           ↓
       DARK <1600   BRIGHT >2000
           ↓           ↓
        Relay ON     Relay OFF
           ↓           ↓
       26-LED ON    26-LED OFF
```

Meanwhile:

```text
ESP32
  ↕
Wi-Fi
  ↕
Blynk Cloud
  ↓
V0 → AUTO / MANUAL
V1 → Manual Lamp
V2 → LDR Value
V3 → System Status
```

**SUMMARY**

This project demonstrates a smart IoT lighting system using an ESP32, LDR sensor, relay, and 26-LED lamp. The LDR continuously senses ambient light, while the ESP32 performs local edge processing to determine whether the environment is dark or bright and automatically controls the lamp through the relay.

The system also integrates Blynk Cloud through Wi-Fi for real-time LDR monitoring and provides AUTO and MANUAL operating modes. In AUTO mode, the ESP32 independently makes the lighting decision; in MANUAL mode, the user can remotely control the lamp through Blynk.
