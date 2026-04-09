#include "OTA.h"

Wireless laptop = Wireless();

void setup() {
  Serial.begin(115200);
  laptop.init();
  laptop.println("Connected Wirelessly");
}

void loop() {
  laptop.handle();

  if (millis() % 10000 == 0) {
    laptop.println(millis());
    Serial.println(millis());
  }

  if (laptop.available() > 0) {
    String beep = laptop.read();
    laptop.print("Echo: ");
    laptop.println(beep);
  }
}