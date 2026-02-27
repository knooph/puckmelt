#include "OTA.h"

//Connects to hotspot using credentials in network.cpp
void OTA_init() {
  Serial.println("Connecting...");
  WiFi.mode(WIFI_STA);

  WiFi.begin(WiFi_SSID,WiFi_PASSWORD);

  int attempt = 0;
  while(WiFi.waitForConnectResult() != WL_CONNECTED && attempt < 10) {
    Serial.println("Connection failed! Retrying...");
    delay(1000);
  }

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Too many retries. Rebooting... (Expect core dump)");
    ESP.restart();
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connection established!");
    Serial.print("Connected to " + String(WiFi.SSID() + "\n"));
  }

  ArduinoOTA.begin();
}