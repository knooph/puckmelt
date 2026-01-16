#include "rc.h"

receiver::receiver() {
  fb = FORWARD_BACWARD;
  lr = LEFT_RIGHT;
  th = THROTTLE_PIN;
  st = STEERING_PIN;
}

void receiver::init() {
  pinMode(fb,INPUT);
  pinMode(lr,INPUT);
  pinMode(th,INPUT);
  pinMode(st,INPUT);
  time = millis();
}
int receiver::getThrottle() {
  unsigned long reading = pulseIn(th,HIGH,40000);
  return constrain(map(reading, 1000, 2000, 0, 100),-100,100);
}
float receiver::getSteering() {
  unsigned long reading = pulseIn(st,HIGH,40000);
  float mapped = constrain(map(reading, 1000, 2000, 0, 100), -1*STEERING_MULT, STEERING_MULT);
  if ( abs(mapped - st_mem) > 4) {
    st_mem = mapped;
    time = millis();
  }
  return mapped;
}
int receiver::getForward() {
  unsigned long reading = pulseIn(fb,HIGH,40000);
  int mapped = constrain(map(reading, 1000, 2000, 100, -100),-100,100);
  if ( abs(mapped - fb_mem) > 4) {
    fb_mem = mapped;
    time = millis();
  }
  return mapped;
}
int receiver::getSideways() {
  unsigned long reading = pulseIn(lr,HIGH,40000);
  int mapped = constrain(map(reading, 1000, 2000, 100, -100),-100,100);
  if ( abs(mapped - lr_mem) > 4) {
    lr_mem = mapped;
    time = millis(); 
  }
  return constrain(map(reading, 1000, 2000, 100, -100),-100,100);
}
bool receiver::selfDestruct() {
  return (millis() - time > 5000);
}