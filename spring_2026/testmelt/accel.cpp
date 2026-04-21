#include "accel.h"

void Accelerometer::init(){
  x1 = LIS331();
  pinMode(10, INPUT); // interrupt
  pinMode(7, OUTPUT); // cs 
  digitalWrite(7, HIGH); // set cs high
  pinMode(6, OUTPUT); // mosi 
  pinMode(5, INPUT); // miso 
  pinMode(4, OUTPUT); // sclk 
  SPI.begin();
  x1.setSPICSPin(7);
  x1.begin(LIS331::USE_SPI);
  x1.setFullScale(LIS331::HIGH_RANGE);
  x1.setHighPassCoeff(LIS331::HPC_16);
  x1.enableHPF(true);
  scale1000[0],scale1000[1],scale1000[2] = 1000,1000,1000;
  offset1000[0],offset1000[1],offset1000[2] = 0,0,0;
  Serial.println("Accelerometer ready!");
}

void Accelerometer::update(){
  x1.readAxes(sensor[X_AXIS],sensor[Y_AXIS],sensor[Z_AXIS]);
}

float Accelerometer::adjustedAccel(AXIS axis){
  float result;
  result = x1.convertToG(LIS331::HIGH_RANGE, sensor[axis]);
  result *= scale1000[axis] / 1000;
  result += offset1000[axis] / 1000;
  return result;
}

float Accelerometer::rawAccel(AXIS axis){
  float result;
  return x1.convertToG(LIS331::HIGH_RANGE,sensor[axis]);
}

void Accelerometer::setScale(AXIS axis, float scale) {
  scale1000[axis] = scale * 1000;
}

void Accelerometer::setOffset(AXIS axis, float offset) {
  scale1000[axis] = offset * 1000;
}