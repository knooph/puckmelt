#include "OTA.h"

#include <BLEDevice.h>
#include <BLESecurity.h>

BLEScanResults* results = nullptr;

class btCallback : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String name = (advertisedDevice.haveName()) ? advertisedDevice.getName() : "UNKNOWN" ;
    String manf = (advertisedDevice.haveManufacturerData()) ? advertisedDevice.getManufacturerData() : "GENERIC";
    String serv = (advertisedDevice.haveServiceData()) ? advertisedDevice.getServiceData() : "UNKNOWN CAPABILITIES";
  }
};

void setup() {
  Serial.begin(115200);
  OTA_init();

  BLEDevice::init("ESP32-Gamepad-Client");                                    // BLEDevice initializer

  BLESecurity *pSecurity = new BLESecurity();

  // Set security capabilities and authentication mode
  pSecurity->setCapability(ESP_IO_CAP_NONE);// Set IO capability to NONE for "Just Works" pairing                                  //Set security protocol thing? IO capability
  pSecurity->setAuthenticationMode(true, false, true);// Bonding, no MITM, secure connections (for "Just Works" pairing)

  // Create scanner and set callbacks
  BLEScan *pBLEScan = BLEDevice::getScan();                                  //This is actually remarkably similiar to NimBLE, get a pointer to the internal scanner object
  pBLEScan->setInterval(1500);
  pBLEScan->setWindow(1000);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);                                                //Start scanning
}

void loop() {
  OTA_handle();
  if (!BLEDevice::getScan()->isScanning()) {
    results->dump();
    results = BLEDevice::getScan()->getResults();
    Serial.println("\nScan Results:");
    for (int i = 0; i < results->getCount(); i++) {
      Serial.print("\t");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(results->getDevice(i).getName());
      Serial.print(" @ ");
      Serial.println(results->getDevice(i).getAddress().toString());
    }
    BLEDevice::getScan()->start(5, false);    //actually do the scan
    delay(5000);
  }
}