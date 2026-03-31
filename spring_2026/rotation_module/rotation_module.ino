#include "OTA.h"
#include "rotation.h"

void setup() {
  Serial.begin(115200);
  //Serial.println("Check A");
  //pinMode(PIN_RGB_LED, OUTPUT);
  //Serial.println("Check B");
  OTA_init();
  //Serial.println("Check C");
}

void loop() {
  OTA_handle();
}