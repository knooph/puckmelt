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
  client.setTimeout(1);
}

String Wireless::read() {
  return client.readString();
}

void Wireless::handle() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    client = server.available();
  
    if (client.available() > 0) {
      int bytesRead = client.readBytes(buffer, (client.available() >= 15) ? 15 : client.available()); //get bytes available for read
      client.flush(); //flush stream in case there was overload
      buffer[bytesRead] = '\0'; //format to be a proper c_string
      client.println(buffer); //print the command so the user can see what they typed

      client.print("Wow, '");
      client.print(buffer);
      client.println("', absolutely riveting!");
      
      memcpy(buffer, "", 15); //clear the buffer
    }
  }
}