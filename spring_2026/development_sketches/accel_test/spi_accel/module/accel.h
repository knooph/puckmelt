#include <Arduino.h>
#include <SPI.h>
#include "SparkFun_LIS331.h"

//int offsetAxisX, offsetAxisY, offsetAxisZ, scaleAxisX, scaleAxisY, scaleAxisZ;

class accel{
  public:
  void init();
  float calculatedAccel(int axis, int scale, int offset);
  float rawAccel(int axis);
};