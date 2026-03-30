#include <Arduino.h>
#include "OTA.h"
#include "rc.h"

receiver* rc = nullptr;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
  rc->init();
}

void loop() {
  OTA_handle();
  rc->init();
}