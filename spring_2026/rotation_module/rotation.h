#pragma once
#include <Arduino.h>

#define TIME_INTVL 1/2400000 //The time between updates in seconds
#define WHEEL_DIST 1 //The distance from the wheel to bot center in meters
#define WHEEL_RADIUS 1 //The effective radius of the wheel.

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
  ANG_V
};

class physicState {
  public:
  static float get(PHYS_VAL index);
  static float x_xl();
  static float y_xl();
  static void update(float nrm_xl, float tan_xl, float z_xl, float rpm);

  private:
  static int16_t state[5];
  static void set(PHYS_VAL index, float value);
};

enum CTRL_MODE {
  ATTACK = false,
  SPRINT = true
};

class controlHandle {
  private:
  static CTRL_MODE mode;
  static uint16_t weapon_rpm;
  static CTRL_MODE get_mode();
  static void set_mode(CTRL_MODE ctrl_mode);
  static float motor_rpm();
};