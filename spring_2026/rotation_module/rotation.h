#pragma once
#include <Arduino.h>

#define TIME_INTVL 1/2400000 //in seconds
#define WHEEL_DIST 1 //distance from the wheel to bot center
#define WHEEL_RADIUS 1 //radius of the wheel
enum PHYS_VAL {
  NRM_XL = 0, //Positive direction is away from axis
  TAN_XL, //Positive direction is cw
  Z_XL,
  ANGL,
  ANG_V
};

class physicState {
  public:
  //[ctr accel, tan accel, z accel, ]
  static float x_xl();
  static float y_xl();
  static float z_xl();
  static float get_heading();
  static void update(int16_t NRM_XL, int16_t TAN_XL, int16_t Z_XL, int16_t ANG_V);

  private:
  static int16_t state[5];
  static int16_t get(PHYS_VAL index);
  static void set(PHYS_VAL index, float value);
};

