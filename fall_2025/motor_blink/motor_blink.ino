#include <Servo.h>

Servo r;
Servo l;

void ramp(int start, int end, int millis) {
  int delay = (end - start)/millis;
  int count = abs(start - end);
  int increment = 0;
  if (start < end) { increment = 1; }
  else { increment = -1; }
  r.write(start);
  for (int i = 0; i < count; i += increment) {
    r.write(start + i);
  }
}

void setup() {
  // put your setup code here, to run once:
  // pinMode(8,OUTPUT);

  pinMode(12, OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  r.attach(10,1000,2000);
  l.attach(11,1000,2000);
  
  // r.write(100);
  // l.write(100);
  // r.write(0);
  // l.write(0);
  // delay(5000);


  // r.write(30);
  // l.write(30);
  // digitalWrite(12,HIGH);
  // digitalWrite(LED_BUILTIN,HIGH);
  // delay(2);
  // r.write(0);
  // l.write(0);
  // delay(500);
  // r.write(20);
  // l.write(20);
  // delay(5000);
  // r.write(0);
  // l.write(0);
  // digitalWrite(12,LOW);
  // digitalWrite(LED_BIULTIN,LOW)

  blinkLED(1); //blink once
  ramp(0,100,2000);
  blinkLED(1); //blink once

  delay(1000);

  blinkLED(2);
  ramp(100,0,4000);
  blinkLED(2);

  delay(1000);

  blinkLED(3);
  ramp(0,20,1000);
  blinkLED(3);

  delay(5000);

  ramp(20,0,1000);
  blinkLED(4);


}

void loop() {
}

void blinkLED(int num) {
  int ctr = 1;
  while (ctr <= num) {
    digitalWrite(12,HIGH);
    delay(100);
    digitalWrite(12,LOW);
    delay(100);
    ctr++;
  }

}