#include "radio.h"
#include "wireless.h"
#include "rotation.h"

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
controlHandler controller; //Converts driver input into useful information for the physicState
physicState puckmath; //does the real-time rotational kinematics using information from the controller. Read target motor throttle off of this object

TaskHandle_t Task0;// Everything on core 0
TaskHandle_t Task1;//Everything on core 1

void setup() {
  Serial.begin(115200); //Don't actually need this, but leaving it here for debug output

  //Initialize modules
  laptop.init();
  radio.init();


  xTaskCreatePinnedToCore(loop0, "Task0", 10000, NULL, 1, &Task0, 0);
  delay(500);
  xTaskCreatePinnedToCore(loop1, "Task1", 10000, NULL, 1, &Task1, 1);

  Serial.end();
}

void loop() {}

//core0 will be dedicated to the tasks that need to be exxecuted in real time like sensor input and motor output.
void loop0(void* pvParameters) {
  
  puckmath.update(0.0,0.0,0.0,0.0); //   <<Y AXIS, X AXIS, Z AXIS, MOTOR RPM
  puckmath.motor_throttle(RIGHT, controller.velocity(), controller.weapon_rpm(), controller.get_offset());// How to getRight motor throttle
  puckmath.motor_throttle(LEFT, controller.velocity(), controller.weapon_rpm(), controller.get_offset());// How to get left motor throttle
}

void loop1(void* pvParameters) {
  radio.handle();
  laptop.handle();
  controller.in(radio.lr_axis, radio.fb_axis, radio.throttle, radio.angle);
}