#pragma once

extern const char* WiFi_SSID; //define in network_id.cpp
extern const char* WiFi_PASSWORD;

// Peripheral 
// └── Service 0x1812 (HID) 
//     ├── Characteristic 0x2A4A (HID Info) 
//     ├── Characteristic 0x2A4B (Report Map) 
//     ├── Characteristic 0x2A4D (Input Report) 
//     │ ├── Descriptor 0x2902 (ClientCharacteristicConfiguration) 
//     │ └── Descriptor 0x2908 (Report Reference) 
//     └── Characteristic 0x2A4C (HIDControlPoint)

enum UUID {
    HID_SERVICE = 0x1812, //Signifies it's an HID Serivce
    HID_INFO = 0x2A4A, // just like basic generic stuff kinda not important
    REPORT_MAP = 0x2A4B, //How the controller buttons are mapped (in the incoming data)
    INPUT_REPORT = 0x2A4D, // A specific input frame of data
    CCCONFIG = 0x2902, //enables the data stream
    REPORT_REF = 0x2908, // specific reference id for input report
    HID_CTRL_PT = 0x2A4C //Idk something that allows the Client to control the peripheral
};