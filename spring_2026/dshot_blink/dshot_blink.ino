#include "OTA.h"
#include <DShotRMT.h>

// Supported: DSHOT150, DSHOT300, DSHOT600, (DSHOT1200)
static constexpr dshot_mode_t DSHOT_MODE = DSHOT300;

//Create motor instance, Takes pin number, DSHOT_MODE, and isBidirectional boolean (default False)
DShotRMT test_motor(4, DSHOT_MODE, true); //Plug in esc to pin 4
dshot_result tele;
void setup() {
  Serial.begin(115200);
  pinMode(PIN_RGB_LED, OUTPUT);
  OTA_init();

  //Init dshot signal
  test_motor.begin();

}

void loop() {

  OTA_handle();

  red_blink(3);
  test_motor.sendThrottlePercent(10);
  delay(1000);
  tele = test_motor.getTelemetry();
  printDShotResult(tele);
  delay(1000);
  test_motor.sendThrottlePercent(0);
  Serial.println(tele.result_code);

  delay(2000);

  red_blink(3);
  test_motor.sendThrottlePercent(-10);
  delay(1000);
  tele = test_motor.getTelemetry();
  printDShotResult(tele);
  delay(1000);
  test_motor.sendThrottlePercent(0);

  delay(5000);
}

void red_blink(int times) {
  for (int i = 0; i < times; i++) {
    rgbLedWrite(PIN_RGB_LED,100,0,0);
    delay(100);
    rgbLedWrite(PIN_RGB_LED,0,0,0);
    delay( (i == times - 1) ? 0 : 100);
  }

}