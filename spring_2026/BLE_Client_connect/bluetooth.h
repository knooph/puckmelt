#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLESecurity.h>
#include <string.h>

//Forward Declarations
class GappCallback; //line 40

struct init_wrapper {
    init_wrapper();
};

class bluetooth {
    private:
        init_wrapper initializer;
        static BLEScan* scanner; //Scanner object pointer
        static BLEAdvertisedDevice* target; //Pointer to target BLE peripheral because it can't be 
        static BLEAddress* target_addr;     //initialized in link time
        BLEAdvertisedDevice target_mem; //actual non-static member that will be pointed to
        BLEAddress target_addr_mem; //actual non-static member that will be pointed to
        BLEScanResults results_mem; //actual non-static member that will be pointed to
        static BLEClient* client; //The client being used to connect to a peripheral (in heap)
        
    public:
        static bool isScanning; //if the device is scanning for peripherals
        static BLEScanResults* results; //point to the internal scan result storage

        bluetooth();
        ~bluetooth();
        void scan(uint32_t seconds);
        void setTarget(std::string mac_addr); //Set target device to connect to based off of addres
        bool setTarget(uint32_t device_index); //set taret device from scan results
    
    //allow Callbacks to access private members
    friend class GappCallback;
};

class GappCallback : public BLEAdvertisedDeviceCallbacks {
    public:
        static int index;
        static std::string output_buffer;
        void onResult(BLEAdvertisedDevice advertisedDevice);
        static void scanCallback(BLEScanResults results);
};

