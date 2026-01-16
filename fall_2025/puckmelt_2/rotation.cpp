#include "rotation.h"

//receiver stuff
motor::motor(int pinNum, int multiplier){
  mult = multiplier;
  pin = pinNum;
}

void motor::init(){
  mr.attach(pin);
  mr.write(100);
  mr.write(0);
}

float motor::setMotorSpeed(int th, int fb, int lr) {
  if (th > THROTTLE_CUTOFF) {
    mr.write( th*THROTTLE_FACTOR + ( fb*sin(heading) + lr*cos(heading) ) * (THROTTLE_FACTOR/9) * mult );
    return th*THROTTLE_FACTOR + ( fb*sin(heading) + lr*cos(heading) ) * (THROTTLE_FACTOR/9) * mult;
  } else {
    mr.write(0);
    return 0;
  }
}

void motor::OFF() {
  mr.write(mr.read() - 1);
}

int motor::getSpeed() {
  return mr.read();
}


//accelerometer
accel::accel() {}
void accel::init() {
  Wire.begin();
  xl.setI2CAddr(0x19);
  xl.begin(LIS331::USE_I2C);
  xl.setFullScale(200);
}
void accel::updateHeading() {
  heading += (millis() - time) * sqrt( sqrt(sq(constrain(readAbs() - 1,0,999999))) * 9.8 * XL_RADIUS );
  while (heading > 2*3.14159) {
    heading -= 2*3.14159;
  }
}
float accel::readAbs() {
  xl.readAxes(xxl,yxl,zxl);
  return xl.convertToG(200, int(sqrt(xxl * xxl + yxl*yxl + zxl*zxl)));
}
float accel::readX() {
  float dummy = readAbs();
  return xl.convertToG(200,xxl);
}
float accel::readY() {
  float dummy = readAbs();
  return xl.convertToG(200,yxl);
}
float accel::readZ() {
  float dummy = readAbs();
  return xl.convertToG(200,zxl);
}