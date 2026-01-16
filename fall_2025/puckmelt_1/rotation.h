#include "settings.h"

double motorSpeed(double weapon_rpm, double* target_vector, bool rightSide);
void updateHeading(unsigned long newTime, float xl);
void setHeading(float newHeading);
float getHeading();