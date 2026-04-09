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
  OTA_init();
}

bool led_on = false;
void loop() {
  OTA_handle();
  control.control_in(0,0,0,0); //receiver input here
  sensor.update(1,1,1); //accelerometer stuff goes here
  r_motor.go(sensor.motor_throttle(RIGHT,control.velocity(),control.weapon_rpm(),control.get_offset()));
  l_motor.go(sensor.motor_throttle(LEFT,control.velocity(),control.weapon_rpm(),control.get_offset()));

  //light is on from 350 to 10 degrees
  if ((sensor.get(ANGL) > 35/18*PI || sensor.get(ANGL) < PI/90) && !led_on) {
    digitalWrite(GPIO_NUM_3,HIGH);
    led_on = true;
  } else if (led_on) {
    digitalWrite(GPIO_NUM_3, LOW);
    led_on = false;
  }
}