#include <Arduino.h>
#include <DShotRMT.h>

const gpio_num_t MOTOR_PIN = GPIO_NUM_4;
DShotRMT motor(MOTOR_PIN, DSHOT150, false);

void setup() {
  Serial.begin(115200);
  motor.begin();
  Serial.println("Motor initialized");
}

void loop() {
  Serial.println("Motor On");

  // send throttle continuously for 10s
  unsigned long t0 = millis();
  while (millis() - t0 < 10000) {
    motor.sendThrottlePercent(25);
    delayMicroseconds(1000);
  }

  Serial.println("Motor Off");

  // send 0 continuously for 2s
  t0 = millis();
  while (millis() - t0 < 2000) {
    motor.sendThrottlePercent(0);
    delayMicroseconds(1000);
  }

  delay(3000);
}
