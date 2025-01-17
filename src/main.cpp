#include <WiFi.h>
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200


#define WIFI_SSID "IoTPrivate"
#define WIFI_PASS "iotprivate303"

// Define pins for the LEDs
#define LED_PIN_1 12
#define LED_PIN_2 14

// Names for the LEDs to be recognized by Alexa
#define LED_1 "led one"
#define LED_2 "led two"
#define BOTH_LEDS "both lamps"

fauxmoESP fauxmo;

// Function to set up Wi-Fi connection
void wifiSetup() {
  // Set Wi-Fi mode to STA (station)
  WiFi.mode(WIFI_STA);

  // Connect to Wi-Fi
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait for the connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Successful connection
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Initialize the serial port
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Connect to Wi-Fi
  wifiSetup();

  // Set up LED pins
  pinMode(LED_PIN_1, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);  // LED off

  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_2, LOW);  // LED off

  // Create and configure the fauxmoESP server
  fauxmo.createServer(true); // not necessary, this is the default value
  fauxmo.setPort(80); // This is required for 3rd generation devices

  // Enable fauxmoESP library
  fauxmo.enable(true);

  // Add virtual devices
  fauxmo.addDevice(LED_1);
  fauxmo.addDevice(LED_2);
  fauxmo.addDevice(BOTH_LEDS);

  // Handler for commands from Alexa
  fauxmo.onSetState([](unsigned char device_id, const char* device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    
    if (strcmp(device_name, LED_1) == 0) {
      // Control LED 1
      Serial.println("LED 1 switched by Alexa");
      digitalWrite(LED_PIN_1, state ? HIGH : LOW);  // Turn LED on/off
    }
    
    if (strcmp(device_name, LED_2) == 0) {
      // Control LED 2
      Serial.println("LED 2 switched by Alexa");
      digitalWrite(LED_PIN_2, state ? HIGH : LOW);  // Turn LED on/off
    }
    
    if (strcmp(device_name, BOTH_LEDS) == 0) {
      // Control both LEDs
      Serial.println("Both LEDs switched by Alexa");
      digitalWrite(LED_PIN_1, state ? HIGH : LOW);  // Turn LED 1 on/off
      digitalWrite(LED_PIN_2, state ? HIGH : LOW);  // Turn LED 2 on/off
    }
  });
}

void loop() {
  // Handle fauxmoESP events
  fauxmo.handle();
}