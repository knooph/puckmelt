#include "OTA.h"
#include "Bluetooth_Module.h"

bluetooth bt;

void setup() {
  delay(1000);
  Serial.begin(115200);
  //bluetooth bt;
  Serial.println("\n\n\n\n\n\n\n\n\n\n");
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();
}

void loop() {
  OTA_handle();
  Serial.println("Working!");
}