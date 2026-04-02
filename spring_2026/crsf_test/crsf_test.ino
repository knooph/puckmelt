#include <Arduino.h>
#include "OTA.h"
#include "rc.h"

receiver* rc = new receiver();

void setup() {
  Serial.begin(115200);
  //pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
  rc->init();
}

void loop() {
  OTA_handle();
  rc_update();
  unsigned long time = millis();
  if (time%1000 == 0) {
    Serial.print("\n");
    Serial.print(receiver::time);
    Serial.print(" ");
    Serial.println(receiver::latency);
    for (uint16_t i : receiver::channels) Serial.println(i);
  }
}