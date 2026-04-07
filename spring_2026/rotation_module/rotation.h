#pragma once
#include <Arduino.h>

#define TIME_INTVL 1/2400000 //The time between updates in seconds
#define XL_RADIUS 0.01 //distance of accelerometer to bot center in meters
#define WHEEL_DIST 1 //The distance from the wheel to bot center in meters
#define WHEEL_RADIUS 1 //The effective radius of the wheel.
#define MAX_RPM_MTR 30000 //the maximum motor speed in rpm
#define FLIP_SENSITIVITY 5 //percent of a 1G to idle in
#define PI 3.14159265359

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
  static void update(float nrm_xl, float tan_xl, float z_xl); //if there's no dshot telemetry
  static float motor_throttle(MOTOR_SIDE side, float v_trans, float weapon_rpm, float angl_offset);

  private:
  static int8_t flip_factor;
  static int16_t state[7];
  static void set(PHYS_VAL index, float value);
};

class controlHandler {
  private:
  static int8_t y_direction;
  static int8_t x_direciton;
  static int8_t x_input;
  static int8_t y_input;
  static int8_t throttle;
  static float get_magnitude(int8_t x_in, int8_t y_in);
  static float get_angle(int8_t x_in, int8_t y_in);

  public:
  static void control_in(int8_t y_dr, int8_t  x_dir, int8_t y_v, int8_t x_v, int8_t th);
  static float velocity();
  static float weapon_rpm();
  static float offset();

  static float rpm_to_v(float rpm);
  static float v_to_rpm(float velocity);
};