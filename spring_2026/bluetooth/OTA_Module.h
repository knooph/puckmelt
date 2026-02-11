#include <Arduino.h>
#include "network_id.h" //include the wifi credentials
#include <WiFi.h> //Library for utilizie the wifi capabilities of the microcontroller
#include <ArduinoOTA.h> //Library for doing Over The Air stuff

void OTA_init();

#define OTA_handle ArduinoOTA.handle