#include "OTA.h"
#include "DShotRMT.h"
/*
motor to test should be plugged into GPIO Pin 4

*/


//DShotRMT(gpio_num_t gpio, dshot_mode_t mode = DSHOT300, bool is_bidirectional = false, uint16_t magnet_count = DEFAULT_MOTOR_MAGNET_COUNT);
DShotRMT motor0 = DShotRMT(GPIO_NUM_4,DSHOT600,false,14);
dshot_result_t status;
bool testActive = false;
unsigned long timeout = 0;

void setup() {
  Serial.begin(115200);
  OTA_init();
  motor_status(motor0.begin(),"Motor Initialize");
}

void loop() {
  OTA_handle();
  Serial_handle();

  if (testActive) {
    csv_telemetry();
    if (millis() >= timeout) {
      Serial.println("%%END TEST%%");
      testActive = false;
    }
  }

}

void Serial_handle() {

  if (Serial.available() == 0) { //If there is no serial input don't do anything
    return;
  }

  String input1 = Serial.readStringUntil(' ');
  String input2 = Serial.readStringUntil('\n');
  Serial.print(input1);
  Serial.print(' ');
  Serial.println(input2);
  float throttle = -1;
  uint time = 0;
  if (input1 == "exit" || input2 == "exit") { // doesn't really do anything except throw an error in PuTTy lol
    Serial.end();
    delay(100);
    Serial.begin();
    return;
  } else if (input1 == "get" || input2 == "get") {
    motor_status( motor0.getTelemetry(), "Retrieve Telemetry");
  } else if ( cleanInput(input1) && cleanInput(input2) ){ //Check if input is formatted correctly
    throttle = input1.toFloat();
    time = abs(input2.toInt());

    if (throttle < 0) { //if throttle is negative, just reverse the motor
      motor0.setMotorSpinDirection(true);
      throttle *= -1;
    } else {
      motor0.setMotorSpinDirection(false);
    }

    testActive = true;
    timeout = millis() + time;
    motor_status(motor0.sendThrottlePercent(throttle),"Command Sent");
    Serial.println("%%START TEST%%");
    Serial.println("time, rpm, voltage(ms), current(mA), consumption(mA), temperature(*C)");
  } else {
    Serial.println("Invalid Command, should be in form 'Throttle_percent_float runtime_millis'");
  }

}

void motor_status(dshot_result_t status, const char* title) {
  Serial.println(title);
  if (status.success) {
    Serial.println("\tSuccessful");
  } else {
    Serial.print("\tError code: ");
    Serial.println(status.result_code);
  }
  if (status.telemetry_available) {
    Serial.print("\teRPM: ");
    Serial.println(status.erpm);
    dshot_telemetry_data data = status.telemetry_data;
    Serial.print("\tRPM ");
    Serial.println(data.rpm);
    Serial.print("\tVoltage(mV) ");
    Serial.println(data.voltage);
    Serial.print("\tCurrent(mA) ");
    Serial.println(data.current);
    Serial.print("\tConsumption(mA)");
    Serial.println(data.consumption);
    Serial.print("\tTemperature(*C)");
    Serial.println(data.temperature);
    Serial.print("\tError flags ");
    Serial.println(data.errors);
  } 
}

void csv_telemetry() {
  dshot_result_t data = motor0.getTelemetry();
  Serial.print(millis());

  if (!data.telemetry_available) { //output 0 if telemetry fails.
    Serial.println(", 0, 0, 0, 0, 0");
    return;
  }
  
  Serial.print(", ");
  Serial.print(data.telemetry_data.rpm);
  Serial.print(", ");
  Serial.print(data.telemetry_data.voltage);
  Serial.print(", ");
  Serial.print(data.telemetry_data.current);
  Serial.print(", ");
  Serial.print(data.telemetry_data.consumption);
  Serial.print(", ");
  Serial.print(data.telemetry_data.temperature);
}

bool cleanInput(String input) {
  return !( (input != "0" && input.toInt() == 0) || input.isEmpty() );
}