/*
  ESP8266 initialisation sketch: WifiManager and OTA.
*/

// Main libraries.
#include <Arduino.h>
#include <ESP8266WiFi.h> // WifiManager, ArduinoOTA
#include <WiFiUdp.h> // ArduinoOTA
#include <DNSServer.h> // WifiManager
#include <ESP8266WebServer.h> // WifiManager
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoOTA.h>
#include <Streaming.h> // Enables the '<<' streaming operator. http://arduiniana.org/libraries/streaming/. PrintEx is similar but not compatible with debug.

// Debugging framework.
const char compileDate[] = __DATE__ " " __TIME__; // Captures date/time that sketch was compiled. Enables version tracking.
#define debug Serial // Select output for debugging: 'Serial'.

// User defined variables (passwords, etc).
char setupAPSSID[] = "BlankESPAP"; // SSID for WiFiManager auto setup AP.
char setupAPPassword[] = "BlankESP"; // Password for WiFiManager auto setup AP.
char wifiHostname[] = "BlankESP";

void wiFiConnect()
// Requires library: ESP8266WiFi.h.
// Uses 'debug' for print output.
{
  WiFi.mode(WIFI_STA); // Sets ESP8266 to station mode only. Improves security and reduces power by enabling auto use of light-sleep mode. http://www.esp8266.com/viewtopic.php?f=32&t=4213&start=10
  WiFi.hostname(wifiHostname);
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);
  // wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 189, 1), IPAddress(192, 168, 189, 1), IPAddress(255, 255, 255, 0)); // Do I need a custom ip for auto config portal?
  if (!wifiManager.autoConnect(setupAPSSID, setupAPPassword)) //fetches SSID and password and tries to connect. If it does not connect it starts an access point with the details specified here and goes into a blocking loop awaiting configuration
  {
    debug.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
  }
  debug.println("WiFi connected");
}

void arduinoOTA()
{
  //ArduinoOTA.setPassword(setupAPPassword);
  ArduinoOTA.onError([](ota_error_t error) {
    debug.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) debug.println("OTA auth failed");
    else if (error == OTA_BEGIN_ERROR) debug.println("OTA begin failed");
    else if (error == OTA_CONNECT_ERROR) debug.println("OTA connect failed");
    else if (error == OTA_RECEIVE_ERROR) debug.println("OTA receive failed");
    else if (error == OTA_END_ERROR) debug.println("OTA end failed");
  });
  ArduinoOTA.begin();
}

void setup()
{
  if (debug == Serial) debug.begin(115200);
  wiFiConnect();
  arduinoOTA();
  debug << "compileDate: " << compileDate << endl;
}

void loop()
{
  ArduinoOTA.handle();
}
