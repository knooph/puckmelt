#include "rotation.h"
#include "settings.h"
#include <math.h>

float current_heading;
unsigned long time;

double motorSpeed(double weapon_rpm, double* target_vector, bool rightSide){
  double velocity_offset = (*target_vector) * cos(current_heading) + (*target_vector + 1) * sin(current_heading); //project target velocity vector to current wheel heading
  
  double base_velocity = weapon_rpm * 2 * 3.14159 * drive_radius;
  base_velocity /= 60; //convert weapon rpm to m/s for the wheel
  
  double motor_velocity = base_velocity;
  if (rightSide) {
    motor_velocity += velocity_offset;
  } else {
    motor_velocity -= velocity_offset;
  }
  return motor_velocity;
}

void updateHeading(unsigned long newTime, float xl) {
  unsigned long delta = newTime - time;
  current_heading += (delta/1000) * sqrt(xl * xl_radius);
  while (current_heading > 2*3.14159) {
    current_heading -= 2*3.14159;
  }
}

void setHeading(float newHeading) {
  current_heading = newHeading;
}

float getHeading() {
  return current_heading;
}

