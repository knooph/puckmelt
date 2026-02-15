#include "OTA.h"

//Connects to hotspot using credentials in network.cpp
void OTA_init() {
  Serial.println("Connecting...");
  WiFi.mode(WIFI_STA);

  WiFi.begin(WiFi_SSID,WiFi_PASSWORD);

  while(WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection failed! Rebooting...");
    ESP.restart();
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connection established!");
    Serial.print("Connected to " + String(WiFi.SSID() + "\n"));
  }

  ArduinoOTA.begin();
}