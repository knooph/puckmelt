#include "OTA.h"
#include "rotation.h"

controlHandler control;
physicState sensor;

class motor { //placeholder class
  public:
  void go(float throttle) {}
};

motor r_motor;
motor l_motor;

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
  control.control_in(1,1,1,1,1); //receiver input here
  sensor.update(1,1,1); //accelerometer stuff goes here
  r_motor.go(sensor.motor_throttle(RIGHT,control.velocity(),control.weapon_rpm(),control.offset()));
  l_motor.go(sensor.motor_throttle(LEFT,control.velocity(),control.weapon_rpm(),control.offset()));
}