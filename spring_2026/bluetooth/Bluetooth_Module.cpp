#include "Bluetooth_Module.h"


NimBLEScan* scanner = nullptr;

bluetooth::bluetooth(){
  NimBLEDevice::init("");
  scanner = NimBLEDevice::getScan();
}

NimBLEScanResults bluetooth::scanFor(int seconds) {
  return scanner->getResults(seconds * 1000); /* Scan for an amount of seconds and outputs scan results to NimBLEScanResults type*/
}

void bluetooth::printScan(NimBLEScanResults results) { /* Get NimBLEScanResults type and print out what was found*/
  int scan_count = results.getCount();
  Serial.println("Scan results:");
  for (int i = 0; i < scan_count; i++) {
    Serial.print(String(i) + ": ");
    Serial.print(results.getDevice(i)->toString().c_str());
    Serial.print("\n");
  }
  Serial.println("");
}