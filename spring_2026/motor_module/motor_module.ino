#include "motor.h"

puckMotor motor = puckMotor(6);
void setup() {
  motor.init();


}
void loop() {
  motor.throttle(10);
}
