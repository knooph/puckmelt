#include "rotation.h"
#include "rc.h"

float heading;
accel xl;
receiver rc;
motor right(RIGHT_MOTOR_PIN,1);
motor left(LEFT_MOTOR_PIN,-1);

void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
  xl.init();
  rc.init();
  right.init();
  left.init();
  heading = 0;
  digitalWrite(LED_PIN,HIGH);
  delay(5000);
}
float right_out;
float left_out;
int th;
int fb;
int lr;
void loop() {
  xl.updateHeading();
  th = rc.getThrottle();
  fb = rc.getForward();
  lr = rc.getSideways();
  heading += rc.getSteering();
  if (!rc.selfDestruct()) {
    right_out = constrain(right.setMotorSpeed(th, lr, fb),0,100);
    left_out = constrain(left.setMotorSpeed(th, lr, fb),0,100);
    Serial.println(String(fb) + " " + String(lr) + " " + String(th) + "\n" + String(right_out) + " " + String(left_out));
  } else {
    right.OFF(); //increment motor down by 1 percent
    left.OFF(); // increment motor down by 1 percent
    Serial.println("Uh oh!\n" + String(right.getSpeed()) + " " + String(left.getSpeed()));
  }
  if ( (heading >= 2*3.14159 - 0.1 || heading <= 0.1) && digitalRead(LED_PIN) == LOW) { digitalWrite(LED_PIN,HIGH); } //led on when within 0.1 radians of 0
  else if (digitalRead(LED_PIN) == HIGH) { digitalWrite(LED_PIN, LOW); }
}
