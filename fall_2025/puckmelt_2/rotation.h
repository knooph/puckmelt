#pragma once

#include "Arduino.h"
#include "Wire.h"
#include "Servo.h"
#include "SparkFun_LIS331.h"
#include "settings.h"

extern float heading; //current angle relative to robot north counterclockwise

class motor {
  private:
    Servo mr;
    int mult;
    int pin;
  public:
    motor(int pinNum, int multiplier);
    void init();
    float setMotorSpeed(int th, int lr, int fb);
    void OFF();
    int getSpeed();
};

class accel {
  unsigned long time;
  int16_t xxl, yxl, zxl;
  LIS331 xl;
  public:
  accel();
  void updateHeading();
  void init();
  float readX();
  float readY();
  float readZ();
  float readAbs();
};