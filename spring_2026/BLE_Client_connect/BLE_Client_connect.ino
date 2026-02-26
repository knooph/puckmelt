#include "OTA.h"
#include <Arduino.h>

#include <BLEDevice.h>
#include <BLESecurity.h>
#include <string.h>

BLEScanResults results;
int glbl_indx = 0;
bool busy = false;
char input[4]; //3 char + null terminator

class btCallback : public BLEAdvertisedDeviceCallbacks {
  public:
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String addr = advertisedDevice.getAddress().toString().c_str();
    String name = (advertisedDevice.haveName()) ? advertisedDevice.getName().c_str(): "UNKNOWN";
    String manf = (advertisedDevice.haveManufacturerData()) ? advertisedDevice.getManufacturerData().c_str() : "UNKNOWN";
    String rssi = (advertisedDevice.haveRSSI()) ? String(advertisedDevice.getRSSI()) : "UNKNOWN" ;
    int srv_ct = advertisedDevice.getServiceUUIDCount();
    glbl_indx++;
    Serial.printf("%d) \tNAME: %s\n\tADDRESS: %s\n\tMANUFACT. DATA: %s\n\tRSSI: %s\n\tSERVICES OFFERED: %d\n",glbl_indx,name,addr,manf,rssi,srv_ct);
    if (srv_ct > 0) {
      Serial.print("\t\t");
      for (int i = 0; i < srv_ct; i++) {
        Serial.print(advertisedDevice.getServiceUUID(i).toString().c_str());
        Serial.print((i == srv_ct-1) ? "\n" : "\n\t\t");
      }
    }

  }

  static void scanCallback(BLEScanResults results) {
    busy = false;
    glbl_indx = 0;
    Serial.println("Scan Complete");
  }

};

class btCnnctCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pClient) {
    busy = false;
    Serial.printf("Connected!");
  }
  void onDisconnect(BLEClient *pClient) {
    Serial.printf("Disconnected!");
  }
};

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5000);
  OTA_init();

  BLEDevice::init("ESP32-Gamepad-Client");                                    // BLEDevice initializer

  BLESecurity *pSecurity = new BLESecurity();

  // Set security capabilities and authentication mode
  pSecurity->setCapability(ESP_IO_CAP_NONE);// Set IO capability to NONE for "Just Works" pairing                                  //Set security protocol thing? IO capability
  pSecurity->setAuthenticationMode(1);// Bonding

  // Create scanner and set callbacks
  BLEScan *pBLEScan = BLEDevice::getScan();                                  //This is actually remarkably similiar to NimBLE, get a pointer to the internal scanner object
  pBLEScan->setInterval(1500);
  pBLEScan->setWindow(1000);
  pBLEScan->setActiveScan(true);
  pBLEScan->setAdvertisedDeviceCallbacks(new btCallback);
  Serial.println("\nScan Results:");
  busy = true;
  busy = pBLEScan->start(5, btCallback::scanCallback, false);                                                //Start scanning
}

void handleInput();
void connectTo(uint8_t deviceIndex);

void loop() {
  OTA_handle();

  // if (!busy) {
  //   Serial.println("\nScan Results:");
  //   busy = true;
  //   BLEDevice::getScan()->start(5, false);    //actually do the scan
  //   delay(5000);
  // }

  if ( (Serial.available() > 0) && (!busy) ) {
    
    int bytesRead = Serial.readBytes(input, 3);
    input[bytesRead] = '\0';
    Serial.println(input);
    handleInput(String(input));
  }
}

void handleInput(String text) {
  if (text == "scn") {
    Serial.println("\nScan Results:");
    glbl_indx = 0;
    busy = true;
    BLEDevice::getScan()->clearResults();
    BLEDevice::getScan()->start(5, btCallback::scanCallback, false);
  } else if (text == "cnx") {
    int bytesRead = Serial.readBytes(input, 3);
    input[bytesRead] = '\0';
    Serial.println(input);
    connectTo( String(input).toInt() );
  } else if (text == "dnx") {
    int bytesRead = Serial.readBytes(input, 3);
    input[bytesRead] = '\0';
    //ADD DISCONNECT LOGIC
  }
}

void connectTo(uint8_t deviceIndex) {
  BLEClient* newClient = BLEDevice::createClient();
  newClient->setClientCallbacks(new btCnnctCallback);
  results = BLEDevice::getScan()->getResults();
  busy = true;
  newClient->connect(&results.getDevice(deviceIndex - 1));
}