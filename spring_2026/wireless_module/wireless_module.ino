#include "wireless.h"

Wireless laptop = Wireless();

void setup() {
  Serial.begin(115200);
  laptop.init();
  laptop.println("Connected Wirelessly");
}

void loop() {
  laptop.handle();
}