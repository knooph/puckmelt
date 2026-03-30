#include "OTA.h"
#include "NimBLEDevice.h"

void setup() {
  Serial.begin(115200);

  Serial.println("Check A");
  pinMode(PIN_RGB_LED, OUTPUT);

  Serial.println("Check B");
  OTA_init();

  Serial.println("Check C");
  NimBLEDevice::init("");

  Serial.println("Check D");
  NimBLEScan *scanner = NimBLEDevice::getScan();
}

void loop() {
  OTA_handle();
}