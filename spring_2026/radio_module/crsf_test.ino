#include <Arduino.h>
#include "wireless.h"
#include "radio.h"

Wireless laptop = Wireless();
receiver* rc = new receiver();

void setup() {
  Serial.begin(115200);
  laptop.init();
  rc->init();
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
    for (int i = 0; i < 4; i++) {
      laptop.println(rc->channels[i]);
    } 
  }
  if (laptop.available() > 0) {
    laptop.println(laptop.read());
    laptop.println("Still here!");
  }
}