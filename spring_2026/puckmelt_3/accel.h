#include <Arduino.h>
#include <SPI.h>
#include "SparkFun_LIS331.h"

enum AXIS {
  X_AXIS = 0,
  Y_AXIS = 1,
  Z_AXIS = 2
};

class accel{
  LIS331 x1;
  int16_t scale1000[3];
  int16_t offset1000[3];
  int16_t sensor[3];
  public:
  void init();
  void update();
  float adjustedAccel(AXIS axis);
  float rawAccel(AXIS axis);
  void setScale(AXIS axis, float scale);
  void setOffset(AXIS axis, float offset);

  friend void handle_terminal();
};