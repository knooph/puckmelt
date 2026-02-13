#include "OTA.h"

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
}

void loop() {
  OTA_handle();
  OTA_handle();
}