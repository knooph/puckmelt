#include "OTA.h"
#include "bluetooth.h"
#include <Arduino.h>

bluetooth bt;

void setup() {
  Serial.begin(115200);
  OTA_init();
}

void loop() {
  OTA_handle();
}