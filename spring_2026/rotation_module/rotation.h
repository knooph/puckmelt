#pragma once
#include <Arduino.h>

#define TIME_INTVL 1/2400000 //The time between updates in seconds
#define XL_RADIUS 0.01 //distance of accelerometer to bot center in meters
#define WHEEL_DIST 1 //The distance from the wheel to bot center in meters
#define WHEEL_RADIUS 1 //The effective radius of the wheel.
#define MAX_RPM_MTR 30000 //the maximum motor speed in rpm
#define FLIP_SENSITIVITY 5 //percent of a 1G to idle in

/*NRM_XL, 0.01
  TAN_XL, 0.01
  Z_XL,   0.01
  ANGL, +- 0.00025
  ANG_V +- 0.0875 */
enum PHYS_VAL {
  NRM_XL = 0, //Positive direction is away from axis
  TAN_XL, //Positive direction is cw
  Z_XL,
  ANGL,
  ANG_V,
  X_V,
  Y_V
};

enum MOTOR_SIDE {
  RIGHT = 1,
  LEFT = -1
};

class physicState {
  public:
  static float get(PHYS_VAL index);
  static float x_xl();
  static float y_xl();
  static void update(float nrm_xl, float tan_xl, float z_xl, float rpm);
  static float motor_rpm(MOTOR_SIDE side, float v_trans, float weapon_rpm, float angl_offset);

  private:
  static int8_t flip_factor;
  static int16_t state[7];
  static void set(PHYS_VAL index, float value);
};

class controlHandler {
  private:
  static uint16_t weapon_rpm;
  float offset;
};