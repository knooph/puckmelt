#include "OTA.h"

Wireless::Wireless() {
  server = WiFiServer(SERVER_PORT);
  SSID = WiFi_SSID;
  PASSWORD = WiFi_PASSWORD;
}

void Wireless::init() {
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

  server.begin();
  Serial.println(WiFi.localIP());
  ArduinoOTA.begin();

  connect();
}

void Wireless::connect() {
  client = server.available();
}

int Wireless::available() {
  return client.available();
}

String Wireless::read() {
  return client.readString();
}

void Wireless::handle() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    client = server.available();
  }
}