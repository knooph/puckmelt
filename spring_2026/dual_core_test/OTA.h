#include <Arduino.h>
#include "network_id.h" //import the wifi credentials
#include <WiFi.h> //Library for utilizie the wifi capabilities of the microcontroller
#include <ArduinoOTA.h> //Library for doing Over The Air stuff

void OTA_init();

#define OTA_handle 

class Wireless{
    WiFiServer server;
    WiFiClient client;
    const char* SSID;
    const char* PASSWORD;
    char buffer[16];

    public:
    Wireless();
    void init();
    void connect();
    String read();
    void handle();

    template<typename T>
    void  println(T data) {
        client.println(data);
    }

    template<typename T>
    void print(T data) {
        client.print(data);
    }
};