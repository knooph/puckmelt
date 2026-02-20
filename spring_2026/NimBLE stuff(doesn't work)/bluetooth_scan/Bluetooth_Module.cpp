#include "Bluetooth_Module.h"

bluetooth::bluetooth() {
  Serial.print("Check A");
  NimBLEDevice::init("");
  Serial.print("Check B");
  scanner = NimBLEDevice::getScan();
  Serial.print("Check C");
}

void bluetooth::scanFor(int seconds) {
  last_scan = scanner->getResults(seconds * 1000);
}

void bluetooth::scanForPrint(int seconds) {
  last_scan = scanner->getResults(seconds * 1000);
  int num = last_scan.getCount();

  Serial.print("Scan Results: ");
  for (int i = 0; i < num; i++) {
    Serial.print(String(i) + ":\n");
    Serial.print(last_scan.getDevice(i)->getName().c_str());
    Serial.print("\n");
  }
}