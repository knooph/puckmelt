#include "OTA.h"

#include <BLEDevice.h>
#include <BLESecurity.h>

BLEScanResults* results = nullptr;

class btCallback : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String addr = advertisedDevice.getAddress().toString();
    String name = (advertisedDevice.haveName()) ? advertisedDevice.getName() : "UNKNOWN";
    String manf = (advertisedDevice.haveManufacturerData()) ? advertisedDevice.getManufacturerData() : "UNKNOWN";
    String rssi = (advertisedDevice.haveRSSI()) ? String(advertisedDevice.getRSSI()) : "UNKNOWN" ;
    int srv_ct = advertisedDevice.getServiceUUIDCount();
    Serial.printf("BLE DEVICE FOUND\n\tNAME: %s\n\tADDRESS: %s\n\tMANUFACT. DATA: %s\n\tRSSI: %s\n\tSERVICES OFFERED: %d\n",name,addr,manf,rssi,srv_ct);
    if (srv_ct > 0) {
      Serial.print("\t\t");
      for (int i = 0; i < srv_ct; i++) {
        Serial.print(advertisedDevice.getServiceUUID(i).toString());
        Serial.print((i == srv_ct-1) ? "\n" : "\n\t\t");
      }
    }

  }
};

class btCnnctCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pClient) { }
  void onDisconnect(BLEClient *pClient) { }
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
  pBLEScan->setAdvertisedDeviceCallbacks(new btCallback);
  Serial.println("\nScan Results:");
  pBLEScan->start(5, false);                                                //Start scanning
}

void loop() {
  OTA_handle();

  if (!BLEDevice::getScan()->isScanning()) {
    Serial.println("\nScan Results:");
    BLEDevice::getScan()->start(5, false);    //actually do the scan
    delay(5000);
  }

}