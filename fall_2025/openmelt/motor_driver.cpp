//this module handles interfacing to the motors
#include "arduino.h"
#include "melty_config.h"
#include "motor_driver.h"
#include "Servo.h"

Servo motor_1;
Servo motor_2;

//intitialize motors
void init_motors(Servo &motor1, Servo &motor2) {
  motor_1 = motor1;
  motor_2 = motor2;
  motor_1.attach(MOTOR_PIN1);
  motor_1.attach(MOTOR_PIN2);

  motor_1.write(255); //calibrate esc
  motor_2.write(255);
  motor_1.write(0);
  motor_2.write(0);
}

//turn motor_X_on (throttle_percent only used for dynamic PWM throttle mode)
void motor_1_on(float throttle_percent) {
  motor_1.write(throttle_percent);
}

void motor_2_on(float throttle_percent) {
  motor_2.write(throttle_percent);
}

//motors shut-down (robot not translating)
void motor_off(Servo &motor) {
  motor.write(0);
}

void motor_1_off() {
  motor_off(motor_1);
}

void motor_2_off() {
  motor_off(motor_2);
}

void motors_off() {
  motor_1_off();
  motor_2_off();
}

//motors coasting (unpowered part of rotation when translating)
void motor_1_coast() {
  motor_1.write(PWM_MOTOR_COAST);
}
void motor_2_coast() {
  motor_2.write(PWM_MOTOR_COAST);
}