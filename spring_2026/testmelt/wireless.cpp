#include "wireless.h"

Wireless::Wireless() {
  server = WiFiServer(SERVER_PORT);
  SSID = WiFi_SSID;
  PASSWORD = WiFi_PASSWORD;
}

/*Initialize wifi and ota libraries*/
void Wireless::init() {
  Serial.println("Connecting...");
  WiFi.mode(WIFI_STA);

  WiFi.begin(WiFi_SSID,WiFi_PASSWORD);

  for (int i = 0; i < 20; i++) { //try maximum of 10 times to connect to wifi
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection failed! Rebooting...");
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

/*connect to laptop (client)*/
void Wireless::connect() {
  client = server.available();
}

/*if stuff in the client serial buffer*/
bool Wireless::available() {
  if (!WiFiEnabled) { return false; } //if wifi never initialized don't do anything
  return (client.peek() != -1);
}

/*read the stuff in client serial buffer*/
String Wireless::read() {
  return client.readString();
}

/*handle wifi updates (run this in loop)*/
void Wireless::handle() {
  if (!WiFiEnabled) { return; } //if wifi never initialized don't don't do anything
  ArduinoOTA.handle();
  if (!client.connected()) {
    client = server.available();
  }
}