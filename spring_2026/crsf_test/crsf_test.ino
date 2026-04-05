#include <Arduino.h>
#include "OTA.h"
#include "rc.h"

Wireless laptop = Wireless();
receiver* rc = new receiver();

void setup() {
  Serial.begin(115200);
  laptop.init();
  rc->init(laptop.getSerial());
}

void loop() {
  laptop.handle();
  rc_update();
  unsigned long time = millis();
  if (time%1000 == 0) {
    laptop.print("\n");
    laptop.print(receiver::time);
    laptop.print(" ");
    laptop.println(receiver::latency);
    for (uint16_t i : receiver::channels) laptop.println(i);
  }
  if (laptop.available() > 0) {
    laptop.println(laptop.read());
    laptop.println("Still here!");
  }
}