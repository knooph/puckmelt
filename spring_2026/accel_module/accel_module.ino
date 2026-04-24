#include "SparkFun_LIS331.h"
#include <SPI.h>
#include "accel.h"


Accelerometer foo;

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
    // String line = String(foo.adjustedAccel(X_AXIS)) + ", " + String(foo.adjustedAccel(Y_AXIS)) +", "+ String(foo.adjustedAccel(Z_AXIS));
    String line = String(foo.rawAccel(X_AXIS)) + ", " + String(foo.rawAccel(Y_AXIS)) +", "+ String(foo.rawAccel(Z_AXIS));
    if (line.compareTo("0.00, 0.00, 0.00") != 0) {
      Serial.println(line);
    }
  }
}
