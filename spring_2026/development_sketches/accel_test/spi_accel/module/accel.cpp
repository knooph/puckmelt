#include "accel.h"
#include <math.h>

LIS331 x1;
int16_t axisX, axisY, axisZ;

void accel::init(){
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
  Serial.println("Accelerometer ready!");
}

float accel::calculatedAccel(int axis, int scale, int offset){
  float result;
  x1.readAxes(axisX, axisY, axisZ);
  if (axis == 0){
    result = x1.convertToG(LIS331::HIGH_RANGE, axisX);
  } else if (axis == 1){
    result = x1.convertToG(LIS331::HIGH_RANGE, axisY);
  } else if (axis == 2){
    result = x1.convertToG(LIS331::HIGH_RANGE, axisZ);
  }
  result *= scale;
  result += offset;
  return result;
}

float accel::rawAccel(int axis){
  float result;
  x1.readAxes(axisX, axisY, axisZ);
  if (axis == 0){
    result = x1.convertToG(LIS331::HIGH_RANGE, axisX);
  } else if (axis == 1){
    result = x1.convertToG(LIS331::HIGH_RANGE, axisY);
  } else if (axis == 2){
    result = x1.convertToG(LIS331::HIGH_RANGE, axisZ);
  }
  return result;
}