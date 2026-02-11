#include "OTA_Module.h"
#include "Bluetooth_Module.h"

bluetooth bt;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
  bt.printScan(bt.scanFor(5));
}

void loop() {
  OTA_handle();
}
