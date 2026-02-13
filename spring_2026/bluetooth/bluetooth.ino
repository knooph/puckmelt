#include "OTA_Module.h"
#include "Bluetooth_Module.h"

bluetooth bt;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
  
  bt.scanForPrint(5);
  
}

void loop() {
  OTA_handle();

  rgbLedWrite(PIN_RGB_LED, 10, 10, 50);
  bt.scanForPrint(5);
  rgbLedWrite(PIN_RGB_LED, 0, 0, 0);
  delay(1000);
}
