#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h> //Library for doing Over The Air stuff
#include <CRSFforArduino.hpp>

//Wireless module settings (edit in .cpp file)
extern const char* WiFi_SSID; //define in network_id.cpp
extern const char* WiFi_PASSWORD;
extern const int SERVER_PORT;
//Rotation module settings that cannot be adjusted at runtime (pre-processor)
#define XL_RADIUS 0.01 //distance of accelerometer to bot center in meters
#define WHEEL_DIST 0.07133 //The distance from the wheel to bot center in meters
#define WHEEL_RADIUS 0.048 //The effective radius of the wheel.
#define PI 3.14159265359

//pre-processor Receiver module settings
#define LEFT_RIGHT_CHANNEL 1
#define FORWARD_BACKWRAD_CHANNEL 2
#define THROTTLE_CHANNEL 4
#define ANGLE_OFFSET_CHANNEL 3

#define RIGHT_MOTOR_PIN 6
#define LEFT_MOTOR_PIN 7