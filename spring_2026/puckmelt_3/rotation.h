#pragma once
#include "settings.h"

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
  static void update(float nrm_xl, float tan_xl, float z_xl, float rpm);
  static void update(float nrm_xl, float tan_xl, float z_xl); //if there's no dshot telemetry
  static float motor_velocity(MOTOR_SIDE side, float v_trans, float weapon_rpm, float angl_offset);

  private:
  static inline int8_t flip_factor;
  static inline int16_t state[7];
  static void set(PHYS_VAL index, float value);

  static float velocity_to_throttle(float v);

  //allows terminal access to private members
  friend void handle_terminal();
};

class controlHandler {
  private:
  static inline int8_t offset;
  static inline int8_t x_input;
  static inline int8_t y_input;
  static inline int8_t throttle;
  static float get_magnitude(int8_t x_in, int8_t y_in);

  public:
  static void in(int8_t x_v, int8_t y_v, int8_t th, int8_t of);
  static float velocity();
  static float weapon_rpm();
  static float get_offset();
  
  static float rpm_to_v(float rpm);
  static float v_to_rpm(float velocity);

  //allows terminal access to private members
  friend void handle_terminal();
};