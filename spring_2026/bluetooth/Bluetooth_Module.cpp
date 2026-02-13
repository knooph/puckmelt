#include "Bluetooth_Module.h"


NimBLEScan* scanner = nullptr;
NimBLEScanResults last_scan;

bluetooth::bluetooth(){
  NimBLEDevice::init("");
  scanner = NimBLEDevice::getScan();
}

void bluetooth::scanFor(int seconds) {
  last_scan = scanner->getResults(seconds * 1000); /* Scan for an amount of seconds and outputs scan results to NimBLEScanResults type*/
}

void bluetooth::scanForPrint(int seconds) { /* Get NimBLEScanResults type and print out what was found*/
  scanFor(seconds);
  int scan_count = last_scan.getCount();
  Serial.println("Scan results:");
  for (int i = 0; i < scan_count; i++) {
    Serial.print(String(i) + ": ");
    Serial.print(last_scan.getDevice(i)->toString().c_str());
    Serial.print("\n");
  }
  Serial.println("");
}