#include "SparkFun_LIS331.h"
#include <SPI.h>
#include "accel.h"

accel foo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  foo.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  static long loopTimer = 0;
  if (millis() - loopTimer > 10)
  {
    loopTimer = millis();
    Serial.println(foo.calculatedAccel(2, 100, 0));
  }
}
