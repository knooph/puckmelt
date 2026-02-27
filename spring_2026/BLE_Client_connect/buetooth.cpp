#include "bluetooth.h"

class GappCallback;

//constructor and initializer's
bluetooth::bluetooth() : target(BLEAdvertisedDevice()), target_addr(BLEAddress("none")) {
    output_buffer = "";
}

void bluetooth::init() {

    BLEDevice::init("ESP32-Gamepad-Client");

    BLESecurity* auth = new BLESecurity(); //set authentification mode and chip capabilities
    auth->setAuthenticationMode(ESP_LE_AUTH_BOND); //Bonding enabled (will seamlessly reconnect I believe)
    auth->setCapability(ESP_IO_CAP_NONE); //Nothing can read or write to/from this device

    scanner = BLEDevice::getScan(); //Get ptr to internal scanning object
    scanner->setInterval(1500);
    scanner->setWindow(1000);
    scanner->setActiveScan(true);
    scanner->setAdvertisedDeviceCallbacks(new GappCallback);
}

void bluetooth::scan(uint32_t seconds) {
    isScanning = scanner->start(seconds, GappCallback::scanCallback, false);
}

void bluetooth::setTarget(std::string mac_addr) {
    target_addr = BLEAddress(mac_addr); //set the target address

    if ( results != nullptr ) { // If scan results are already in check if a matching address
        for (int i = 0; i < results->getCount(); i++) {
            if (results->getDevice(i).getAddress().equals(target_addr)) {
                target = results->getDevice(i);
            }
        }
    }
}

bool bluetooth::setTarget(uint32_t device_index) {
    if ( results != nullptr ) {
        target = results->getDevice(device_index);
        target_addr = target.getAddress();
    }
}

class GappCallback : public BLEAdvertisedDeviceCallbacks {
    public:
    void onResult(BLEAdvertisedDevice advertisedDevice) {}
    static void scanCallback(BLEScanResults results) { bluetooth::isScanning = false; }
};

