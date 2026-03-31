#include <Arduino.h>
#include "OTA.h"
#include "rc.h"

receiver* rc = new receiver();

void setup() {
  Serial.begin(115200);
  //pinMode(PIN_RGB_LED, OUTPUT);
  //OTA_init();
  rc->init();
}

void loop() {
  OTA_handle();
  Serial.print("Throttle ");
  Serial.println(rc->throttle);
  Serial.print("FB ");
  Serial.println(rc->fb_axis);
  Serial.print("LR ");
  Serial.println(rc->lr_axis);
}