/************************************************************
 * Maincrafts Internship - Embedded Systems & IoT Task-06
 *
 * Project:
 * ESP32 Smart Street Light
 * Edge Decision Making + Hybrid Auto/Manual IoT Control
 *
 * Hardware:
 * ESP32 DevKit V1
 * LDR
 * 10k resistor
 * 5V Relay Module
 * Handmade 26 x White LED Lamp
 *
 * Blynk:
 * V0 = Auto/Manual Mode
 * V1 = Manual Lamp Control
 * V2 = LDR Sensor Value
 * V3 = System Status
 ************************************************************/

#define BLYNK_TEMPLATE_ID   "TMPL3zbJbpcI6"
#define BLYNK_TEMPLATE_NAME "ESP32  Hybrid IoT System  Smart Street Light""
#define BLYNK_AUTH_TOKEN    "pVD-smeGQZ-4YZFyV2e9KgTdnT6YwM_E"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// ---------------- Wi-Fi ----------------

char ssid[] = "TP-Link_D848";
char pass[] = "13185787";

// ---------------- Pin Configuration ----------------

#define LDR_PIN    34
#define RELAY_PIN  26

// ---------------- Relay Logic ----------------

// Most 5V relay modules are ACTIVE LOW.
// LOW  = Relay ON
// HIGH = Relay OFF

#define RELAY_ON   LOW
#define RELAY_OFF  HIGH

// ---------------- LDR Thresholds ----------------

// Because our LDR divider is:
// 3.3V -> LDR -> GPIO34 -> 10k -> GND
//
// Dark  = LOW ADC value
// Bright = HIGH ADC value

// Hysteresis prevents rapid relay switching.

const int DARK_THRESHOLD  = 1600;
const int LIGHT_THRESHOLD = 2000;

// ---------------- Variables ----------------

bool autoMode = true;
bool manualLampState = false;
bool lampState = false;

int ldrValue = 0;

unsigned long lastSensorRead = 0;

const unsigned long SENSOR_INTERVAL = 500;

// Blynk Timer
BlynkTimer timer;


// ==========================================================
// Function: Control Relay
// ==========================================================

void setLamp(bool state)
{
  lampState = state;

  if (state)
  {
    digitalWrite(RELAY_PIN, RELAY_ON);
  }
  else
  {
    digitalWrite(RELAY_PIN, RELAY_OFF);
  }

  Serial.print("Lamp: ");

  if (state)
    Serial.println("ON");
  else
    Serial.println("OFF");
}


// ==========================================================
// AUTO MODE - Edge Decision
// ==========================================================

void automaticDecision()
{
  /*
   * This decision is made LOCALLY inside ESP32.
   * No cloud command is required.
   */

  if (lampState == false)
  {
    // Lamp currently OFF
    // Turn ON when environment becomes dark

    if (ldrValue < DARK_THRESHOLD)
    {
      setLamp(true);

      Serial.println("EDGE DECISION: DARK -> Lamp ON");
    }
  }
  else
  {
    // Lamp currently ON
    // Turn OFF only when sufficiently bright

    if (ldrValue > LIGHT_THRESHOLD)
    {
      setLamp(false);

      Serial.println("EDGE DECISION: BRIGHT -> Lamp OFF");
    }
  }
}


// ==========================================================
// Sensor Processing
// ==========================================================

void readSensorAndProcess()
{
  ldrValue = analogRead(LDR_PIN);

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // --------------------------------------------------------
  // AUTO MODE
  // --------------------------------------------------------

  if (autoMode)
  {
    automaticDecision();
  }

  // --------------------------------------------------------
  // Send sensor value to Blynk
  // --------------------------------------------------------

  Blynk.virtualWrite(V2, ldrValue);

  // --------------------------------------------------------
  // Send system status
  // --------------------------------------------------------

  if (autoMode)
  {
    if (lampState)
      Blynk.virtualWrite(V3, "AUTO - DARK - LAMP ON");
    else
      Blynk.virtualWrite(V3, "AUTO - BRIGHT - LAMP OFF");
  }
  else
  {
    if (lampState)
      Blynk.virtualWrite(V3, "MANUAL - LAMP ON");
    else
      Blynk.virtualWrite(V3, "MANUAL - LAMP OFF");
  }
}


// ==========================================================
// BLYNK: AUTO / MANUAL MODE
// V0
// ==========================================================

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  if (value == 1)
  {
    autoMode = false;

    Serial.println();
    Serial.println("MODE: MANUAL");

    // When entering manual mode,
    // use the current manual switch state.

    setLamp(manualLampState);
  }
  else
  {
    autoMode = true;

    Serial.println();
    Serial.println("MODE: AUTO");

    // Immediately allow ESP32 to make a local decision.
    automaticDecision();
  }
}


// ==========================================================
// BLYNK: MANUAL LAMP CONTROL
// V1
// ==========================================================

BLYNK_WRITE(V1)
{
  manualLampState = param.asInt();

  Serial.print("Manual command: ");

  if (manualLampState)
    Serial.println("ON");
  else
    Serial.println("OFF");

  // Manual command is accepted only in MANUAL mode.

  if (!autoMode)
  {
    setLamp(manualLampState);
  }
  else
  {
    Serial.println("Ignored: System is currently in AUTO mode.");
  }
}


// ==========================================================
// SETUP
// ==========================================================

void setup()
{
  Serial.begin(115200);

  delay(500);

  Serial.println();
  Serial.println("======================================");
  Serial.println(" Maincrafts Task-06");
  Serial.println(" Smart Edge IoT System");
  Serial.println("======================================");

  // Relay configuration
  pinMode(RELAY_PIN, OUTPUT);

  // Start with lamp OFF
  digitalWrite(RELAY_PIN, RELAY_OFF);

  // LDR ADC
  analogReadResolution(12);

  Serial.println("Relay initialized.");
  Serial.println("Lamp initially OFF.");

  // Connect Blynk + Wi-Fi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Connecting to Blynk...");

  // Read/process sensor every 500 ms
  timer.setInterval(SENSOR_INTERVAL, readSensorAndProcess);

  Serial.println("System ready.");
}


// ==========================================================
// LOOP
// ==========================================================

void loop()
{
  Blynk.run();
  timer.run();
}