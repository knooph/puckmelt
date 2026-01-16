#include <H3LIS331DL.h> //Header file that will manage accelerometer things, it also includes <Wire.h>
#include "settings.h"
#include "rotation.h"
#include "SparkFun_LIS331.h"
#include "Servo.h"
#include <Wire.h>

LIS331 xl; //create instance of accelerometer
Servo rmotor;
Servo lmotor;

double target_vector[2] = {1, 0};
int16_t xxl, yxl, zxl;
int go;

void calibrateServo() {
  rmotor.attach(9);
  lmotor.attach(10);

  rmotor.write(100);
  lmotor.write(100);
  rmotor.write(0);
  lmotor.write(0);
}

void calibrateXL() {
  Wire.begin();
  xl.setI2CAddr(0x19);
  xl.begin(LIS331::USE_I2C);
  xl.setFullScale(200);
}

int readDrive(int ch) {
  unsigned long reading = pulseIn(ch,HIGH,40000); // rc pulse sent every 20 ms ig?
  return constrain(map(reading, 1000, 2000, 100, -100),-100,100); //scales reading to throttle percent
}
int readThrottle(int ch) {
  unsigned long reading = pulseIn(ch,HIGH,40000); // rc pulse sent every 20 ms ig?
  return constrain(map(reading, 1000, 2000, 0, 100),0,100); //scales reading to throttle percent
}

void setup() {

  Serial.begin(9600);

  setHeading(0);
  pinMode(8,OUTPUT); //led pin
  pinMode(CH1, INPUT); //ch1 left/right
  pinMode(CH2, INPUT); //ch2 forward/backward
  pinMode(CH3, INPUT); //ch3 throttle

  calibrateXL();
  calibrateServo();
}

void loop() {
  xl.readAxes(xxl, yxl, zxl);
  xxl = xl.convertToG(200,xxl);
  yxl = xl.convertToG(200,xxl);
  zxl = xl.convertToG(200,xxl);
  updateHeading(millis(), xxl); //update heading based off acceleration data IMPORTANT FIGURE OUT WHICH AXIS IS CORRECT

  target_vector[0] = readDrive(CH2);
  target_vector[1] = readDrive(CH1);
  go = readThrottle(CH3);
  int rv = motorSpeed(go * 36, target_vector, false); //set left motor speed
  int lv = motorSpeed(go * 36, target_vector, true);
  Serial.println(String(target_vector[0]) + " " + String(target_vector[1]) + " " + String(go));
  Serial.println(String(rv) + " " + String(lv) + "\n");
  if ( (getHeading() >= 2*3.14159 - 0.1 || getHeading() <= 0.1) && digitalRead(9) == LOW) { digitalWrite(9,HIGH); } //led on when within 0.1 radians of 0
  else if (digitalRead(9) == HIGH) { digitalWrite(9, LOW); }
}