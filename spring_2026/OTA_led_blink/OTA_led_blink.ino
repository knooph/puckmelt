#include <WiFi.h> //Library for utilizie the wifi capabilities of the microcontroller
#include <ArduinoOTA.h> //Library for doing Over The Air stuff


//WiFi credentials
const char* hotspot_name = "Puckmelt_OTA"; //the SSID (Service Set Identifier) is the name of the WiFi network you want to connect to.
const char* password = "5hit4boutD1ck";

void setup() {
  
  WiFi.begin(hotspot_name,password); //connects to the hotspot
  while (WiFi.status() != WL_CONNECTED) { //wait until connected to hotspot
    delay(500);
  }
  ArduinoOTA.begin(); //Init library

  pinMode(PIN_RGB_LED, OUTPUT);
}

void loop() {
  ArduinoOTA.handle(); //idk what this does exactly? It probably just listens for a specific signal over the wifi or something to quit the program and start uploading

  if (digitalRead(PIN_RGB_LED) == HIGH && millis() % 1000 == 0) { //Swap pin from high to low at every 1 second
    rgbLedWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS);
  } else if (digitalRead(PIN_RGB_LED) == LOW && millis() % 1000 == 999) { //Swap pin from low to high every 0.999 second
    rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);
  }

}