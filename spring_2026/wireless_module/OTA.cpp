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

  for (int i = 0; i < 20; i++) { //try maximum of 10 times to connect to wifi
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection failed! Rebooting...");
      ESP.restart();
    } else if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      Serial.println("Connection established!");
      Serial.print("Connected to " + String(WiFi.SSID() + "\n"));
      WiFiEnabled = true;
      break;
    }

    if (i == 19 && WiFi.status() != WL_CONNECTED) {
      Serial.println("Giving up with wifi connection.");
      return;
    }
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
  if (!WiFiEnabled) { return 0; } //if wifi never initialized don't do anything
  return client.available();
}

String Wireless::read() {
  return client.readString();
}

void Wireless::handle() {
  if (!WiFiEnabled) { return; } //if wifi never initialized don't don't do anything
  ArduinoOTA.handle();
  if (!client.connected()) {
    client = server.available();
  }
}