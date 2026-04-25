#include "accel.h"
#include <math.h>

void Accelerometer::init(){
  x1 = LIS331();
  pinMode(INT1_PIN, INPUT); // interrupt
  pinMode(SS_PIN, OUTPUT); // ss 
  digitalWrite(SS_PIN, HIGH); // set ss high
  pinMode(MOSI_PIN, OUTPUT); // mosi 
  pinMode(MISO_PIN, INPUT); // miso 
  pinMode(SCK_PIN, OUTPUT); // sck 
  SPI.begin(SCK_PIN,MISO_PIN,MOSI_PIN,SS_PIN);
  x1.setSPICSPin(SS_PIN);
  x1.begin(LIS331::USE_SPI);
  x1.setFullScale(LIS331::HIGH_RANGE);
  // x1.setHighPassCoeff(LIS331::HPC_16);
  // x1.enableHPF(true);
  scale1000[0] = 1000;
  scale1000[1] = 1000;
  scale1000[2] = 1000;
  offset1000[0] = 0;
  offset1000[1] = 0;
  offset1000[2] = 0;
  delay(100);
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
  return sensor[axis];
}

void Accelerometer::setScale(AXIS axis, float scale) {
  scale1000[axis] = scale * 1000;
}

void Accelerometer::setOffset(AXIS axis, float offset) {
  offset1000[axis] = offset * 1000;
}