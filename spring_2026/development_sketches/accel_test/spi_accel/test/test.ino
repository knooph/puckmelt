#include "SparkFun_LIS331.h"
#include <SPI.h>

LIS331 x1;

void setup() {
  // put your setup code here, to run once:
  pinMode(10, INPUT); // interrupt
  pinMode(7, OUTPUT); // cs 
  digitalWrite(7, HIGH); // set cs high
  pinMode(6, OUTPUT); // mosi 
  pinMode(5, INPUT); // miso 
  pinMode(4, OUTPUT); // sclk 
  SPI.begin();
  x1.setSPICSPin(7);
  x1.begin(LIS331::USE_SPI);
  x1.setFullScale(HIGH_RANGE);
  //x1.intSrcConfig(LIS331::INT_SRC, 1); 
  //x1.setIntDuration(50, 1);
  //x1.setIntThreshold(2,1);
  //x1.enableInterrupt(LIS331::Z_AXIS, LIS331::TRIG_ON_HIGH, 1, true);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  static long loopTimer = 0;
  int16_t x, y, z;
  if (millis() - loopTimer > 10)
  {
    loopTimer = millis();
    x1.readAxes(x, y, z);

    Serial.println(x1.convertToG(24,x));
    Serial.println(x1.convertToG(24,y));
    Serial.println(x1.convertToG(24,z));
    Serial.println(" ");
  }
  //if (digitalRead(10) == HIGH)
  //{
  //  Serial.println("Interrupt");
  //}
}
