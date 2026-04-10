#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h> //Library for doing Over The Air stuff
#include <CRSFforArduino.hpp>
#include <string.h>

//Wireless module settings (edit in .cpp file)
extern const char* WiFi_SSID; //define in network_id.cpp
extern const char* WiFi_PASSWORD;
extern const int SERVER_PORT;

//Rotation module settings that can be adjusted at runtime
extern double TIME_INTVL;//The time between updates in seconds
extern uint MAX_RPM_MTR; //the maximum motor speed in rpm
extern uint8_t FLIP_SENSITIVITY; //percent of a 1G to idle in

//Rotation module settings that cannot be adjusted at runtime (pre-processor)
#define XL_RADIUS 0.01 //distance of accelerometer to bot center in meters
#define WHEEL_DIST 0.07133 //The distance from the wheel to bot center in meters
#define WHEEL_RADIUS 0.048 //The effective radius of the wheel.
#define PI 3.14159265359

//terminal access functions
class SETTINGS_ACCESS {
    static void max_rpm(uint value) {MAX_RPM_MTR = value;}
    static void time_int(double value){TIME_INTVL = value;}
    static void flip_sens(uint8_t value) {FLIP_SENSITIVITY = value;}
    friend void handle_terminal();
};

extern String graphic;