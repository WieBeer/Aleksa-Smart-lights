# ESP32 Alexa Controlled LEDs
This code allows an ESP32 device to connect to Wi-Fi and control two LEDs using voice commands via Amazon Alexa. The code utilizes the fauxmoESP library to simulate the presence of smart devices that Alexa can interact with. Here's a detailed breakdown of the code and instructions for setting it up:
___
### Prerequisites
1. Hardware:
   - ESP32 development board
   - Two LEDs
   - Relay module
   - Breadboard and jumper wires

2. Software:
   - Visual S with ESP32 board support installed
   - fauxmoESP library (can be installed via the Arduino Library Manager)
___     
#### Libraries and Constants
```
#include <WiFi.h>
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

#define WIFI_SSID "IoTPrivate"
#define WIFI_PASS "iotprivate303"
#define LED_PIN_1 12
#define LED_PIN_2 14

#define LED_1 "led one"
#define LED_2 "led two"
#define BOTH_LEDS "both lamps"
```

- Libraries: Includes the necessary libraries for Wi-Fi and the fauxmoESP functionality.
- Constants: Defines Wi-Fi credentials, LED pins, and device names recognized by Alexa.
___
#### Wi-Fi Setup

```
  void wifiSetup() {
  WiFi.mode(WIFI_STA);
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {-   Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  }
```
- Sets the Wi-Fi mode to station mode.
- Attempts to connect to the specified Wi-Fi network.
- Prints the connection status to the serial monitor.
  ___
  #### Setup Function

  ```
  void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  wifiSetup();

  pinMode(LED_PIN_1, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_2, LOW);

  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);

  fauxmo.addDevice(LED_1);
  fauxmo.addDevice(LED_2);
  fauxmo.addDevice(BOTH_LEDS);

  fauxmo.onSetState([](unsigned char device_id, const char* device_name, bool state, unsigned char value) {
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if (strcmp(device_name, LED_1) == 0) {
      Serial.println("LED 1 switched by Alexa");
      digitalWrite(LED_PIN_1, state ? HIGH : LOW);
    }
    if (strcmp(device_name, LED_2) == 0) {
      Serial.println("LED 2 switched by Alexa");
      digitalWrite(LED_PIN_2, state ? HIGH : LOW);
    }
    if (strcmp(device_name, BOTH_LEDS) == 0) {
      Serial.println("Both LEDs switched by Alexa");
      digitalWrite(LED_PIN_1, state ? HIGH : LOW);
      digitalWrite(LED_PIN_2, state ? HIGH : LOW);
    }
  });
}

- Initializes the serial communication.
- Calls the Wi-Fi setup function.
- Sets the LED pins as outputs and turns them off initially.
- Configures and enables the fauxmoESP server.
- Adds virtual devices for Alexa to recognize and control.
- Defines a callback function to handle state changes triggered by Alexa.
___

#### Loop Function

```
void loop() {
  fauxmo.handle();
}
```

- Continuously handles fauxmoESP events.
___
### Setup Instructions

**1. Hardware Connection:**
   - Connect the LEDs to the ESP32 GPIO pins specified in the code (`LED_PIN_1` and `LED_PIN_2`).
   - Connect the other end of each LED to the ground via a relay.

**2. Software Configuration:**
   - Open the Visual Studio Code and load the code.
   - Make sure the ESP32 board is selected in the board manager.
   - Install the fauxmoESP library if not already installed 
   
**3. Upload and Test:**
   - Upload the code to the ESP32.
   - Open the serial monitor to check the Wi-Fi connection status and IP address.
   - Discover new devices using the Alexa app on your smartphone.

**4. Control LEDs:**
   - Use voice commands like "Alexa, turn on led one" to control the LEDs.
