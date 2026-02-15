#include "OTA.h"
#include "Bluetooth_Module.h"

bluetooth bt;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n\n\n\n\n\n");
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
}

void loop() {
  OTA_handle();
  Serial.println("Working!");
}