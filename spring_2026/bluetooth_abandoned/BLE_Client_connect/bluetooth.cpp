#include "bluetooth.h"

/* INIT_WRAPPER ---
    Inlcuded as a member of the bluetooth class with a default constructor. The entire point of this struct is just to allow the BLE library to be initialized before
    the bluetooth constructor. other some members can't be properly stored in memory or something and arduino doesn't work
*/

//Small struct w/ default constructor to initialize BLE Library before bluetooth class constructor
init_wrapper::init_wrapper() {
    if (!BLEDevice::getInitialized()) { BLEDevice::init("Puckmelt-Client"); } 
}



/* GAPPCALLBACK ---
    This class handles callback events for whenever a ble device is found or a scan is complete. Does not need to be instantiated. Interacts largely with the main bluetooth class
*/

//Bluetooth Device Found callback. If target device is found during scan connect to it.
void GappCallback::onResult(BLEAdvertisedDevice advertisedDevice) { //Callback for finding a BLE device
    if( advertisedDevice.getAddress().equals(*bluetooth::target_addr) ) {
        if (bluetooth::client->isConnected()) {//disconnect if still configured as connected
            bluetooth::client->disconnect();
        }
        bluetooth::scanner->stop(); //Stop any active scans
        bluetooth::client->connect(&advertisedDevice); //connect
    }
    if (!bluetooth::isScanning) { bluetooth::isScanning = true; }
}

//Scan completion callback. just stores stuff
void GappCallback::scanCallback(BLEScanResults results) {
    bluetooth::isScanning = false;
    *bluetooth::results = results; // store scan results upon complete
}


/* BLUETOOTH CLASS ---
    This is the main handler of bluetooth connection and events for Puckmelt.
*/

//Static Private members
BLEScan*                bluetooth::scanner = nullptr; //Need to declare static members outside of class so linker doesn't complain
BLEAdvertisedDevice*    bluetooth::target = nullptr; //As static members they need to be allocated in memory but if there is no instance of the class it might never happen
BLEAddress*             bluetooth::target_addr = nullptr;
BLEClient*              bluetooth::client = nullptr;

//Static Public members
bool                    bluetooth::isScanning = false;
BLEScanResults*         bluetooth::results = nullptr;


//Constructor: Initializes Class
bluetooth::bluetooth() : initializer(init_wrapper()),target_addr_mem(BLEAddress("none")){

    BLESecurity* auth = new BLESecurity(); //set authentification mode and chip capabilities
    auth->setAuthenticationMode(ESP_LE_AUTH_BOND); //Bonding enabled (will seamlessly reconnect I believe)
    auth->setCapability(ESP_IO_CAP_NONE); //Nothing can read or write to/from this device

    scanner = BLEDevice::getScan(); //Get ptr to internal scanning object
    scanner->setInterval(100);
    scanner->setWindow(100);
    scanner->setActiveScan(true); //forgot
    scanner->setAdvertisedDeviceCallbacks(new GappCallback); //when a device is found call GappCallback::onResult

    //Initialize internal memory
    target_mem = BLEAdvertisedDevice();
    results_mem = BLEScanResults();

    //Initialize static members
    target = &target_mem;
    target_addr = &target_addr_mem;
    results = &results_mem;

    client = new BLEClient();
}

//Destructor
bluetooth::~bluetooth() { delete client; }

//Scan for an amount of seconds. ptr to scan is sent to bluetooth.results.
void bluetooth::scan(uint32_t seconds) { //Start a scan with callbacks
    isScanning = scanner->start(seconds, GappCallback::scanCallback, false);
}

//Specifies an address of device to connect to.
void bluetooth::setTarget(std::string mac_addr) {

    target_addr_mem = BLEAddress(String(mac_addr.c_str())); //set the target address
    for (int i = 0; i < results->getCount(); i++) { //Scan through scan results to check for device with address, if found then set it as the target
        if (results->getDevice(i).getAddress().equals(*target_addr)) {
            target_mem = results->getDevice(i);
        }
    }
}

//Specifies a device within the scan results to connect to
bool bluetooth::setTarget(uint32_t device_index) { //set target to device index in scan
    if (device_index < results->getCount()) {
        target_mem = results->getDevice(device_index);
        target_addr_mem = target->getAddress();
        return true;
    } else {
        return false;
    }
}