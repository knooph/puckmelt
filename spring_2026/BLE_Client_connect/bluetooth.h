#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLESecurity.h>
#include <string.h>

class bluetooth {
    private:
        BLEScan* scanner = nullptr;
        BLEAdvertisedDevice target;
        BLEAddress target_addr;
        BLEScanResults* results = nullptr;
        std::string output_buffer;
    public:
        static bool isScanning;
        bluetooth();
        void init();
        void scan(uint32_t seconds);
        void setTarget(std::string mac_addr);
        bool setTarget(uint32_t device_index);
};