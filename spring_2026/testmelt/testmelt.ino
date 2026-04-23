#include "radio.h"
#include "wireless.h"
#include "rotation.h"
#include "accel.h"
#include "MOTOR_PLACEHOLDER.h"

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
controlHandler controller; //Converts driver input into useful information for the physicState
physicState puckmath; //does the real-time rotational kinematics using information from the controller. Read target motor throttle off of this object
Accelerometer xl = Accelerometer();

MOTOR_PLACEHOLDER rmotor;
MOTOR_PLACEHOLDER lmotor;

TaskHandle_t main_loop;

void handle_terminal();//forward declaration

void setup() {
  Serial.begin(115200); //Don't actually need this, but leaving it here for debug output

  //Initialize modules
  laptop.init();
  radio.init();
  xl.init();

  laptop.println("Enter Test number into puckmelt_terminal");
  Serial.end();



  while (laptop.available() <= 0) {
    delay(10);
  }
  int test_case = laptop.read().toInt();

  switch (test_case) {
    case 1:
      xTaskCreatePinnedToCore(wireless_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
    case 2:
      xTaskCreatePinnedToCore(radio_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
    case 3:
      xTaskCreatePinnedToCore(motor_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
    case 4:
      xTaskCreatePinnedToCore(radio_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
    case 5:
      xTaskCreatePinnedToCore(radio_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
  }
}

void loop() {}

void wireless_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    handle_terminal();
  }
  vTaskDelete(NULL); //if the task ends delete it
}

void radio_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.handle();
    handle_terminal();

    radio.batt_telemetry(1,2,3,4);
    String data = String(radio.fb_axis) + "," + String(radio.lr_axis) + "," + String(radio.angle) + "," + String(radio.throttle);
    //send forward joystick, sideways joystick, angle offset, throttle
    laptop.println(data);
  }
  vTaskDelete(NULL); //if the task ends delete it
}

void motor_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.handle();
    handle_terminal();
    int r = 0;
    int l = 0;
    if (millis() < 35000) {
      r = 10;
      l = 10;
    }
    rmotor.GO(r);
    lmotor.GO(l);
    String data = String(l) + "," + String(r);
    radio.batt_telemetry(1,2,3,4);
    laptop.println(data);
  }
  vTaskDelete(NULL); //if the task ends delete it
}

void accel_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.handle();
    handle_terminal();


  }
  vTaskDelete(NULL); //if the task ends delete it
}

void rotation_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.handle();
    handle_terminal();
  }
  vTaskDelete(NULL); //if the task ends delete it
}
void handle_terminal() {
  if (!laptop.WiFiEnabled || !laptop.client.connected()) { return; } //quit if wifi is not enabled or client isnt connected

  WiFiClient* out = &laptop.client;
  String buffer;
  bool execute = false;
  int end = buffer.length() - 1;

  if (out->available() > 0) { //read into the buffer
    buffer += out->readString();
  }


  //EXECUTE LOGIC
  if (buffer.charAt(end) == '\n' || buffer.charAt(end) == '\r') { //don't execute until user presses enter
    buffer.trim();

    if (buffer.compareTo("kill") == 0) { // "kill"
      radio.watchdog_enable = true;//just set everything to 0
      radio.throttle = 0;
      radio.fb_axis = 0;
      radio.lr_axis = 0;
      controller.throttle = 0;
      controller.x_input = 0;
      controller.y_input = 0;

    } else if (buffer.compareTo("restart") == 0) { // "restart"
      ESP.restart();//restart the entire program, only way to un-kill

    } else if (buffer.startsWith("configure ")) {
                    //change a value during runtime.
      if (buffer.substring(10).startsWith("maxrpm ")) { // "configure maxrpm 200"
        uint new_value = buffer.substring(10).substring(7).toInt();
        SETTINGS_ACCESS::max_rpm(new_value);
      } else if (buffer.substring(10).startsWith("timeint ")) { // "configure timeint 0.00000000021"
        double new_value = buffer.substring(10).substring(8).toDouble();
        SETTINGS_ACCESS::time_int(new_value);
      } else if (buffer.substring(10).startsWith("flipsens ")) { // "configure flipsens"
        uint8_t new_value = buffer.substring(10).substring(9).toInt();
        SETTINGS_ACCESS::flip_sens(new_value);
      }
    } else if (buffer.startsWith("accel ")) { // accel scale x 2.4
      if (buffer.substring(6).startsWith("scale ")) {
        AXIS trgt_axs = X_AXIS;

        switch (buffer.charAt(12)) {
          case 'x':
            trgt_axs = X_AXIS;
            break;
          case 'y':
            trgt_axs = Y_AXIS;
            break;
          case 'z':
            trgt_axs = Z_AXIS;
            break;
        }

        float new_value = buffer.substring(14).toFloat();
        xl.setScale(trgt_axs, new_value);

      } else if (buffer.substring(6).startsWith("offset ")) {
        AXIS trgt_axs = X_AXIS;

        switch (buffer.charAt(13)) {
          case 'x':
            trgt_axs = X_AXIS;
            break;
          case 'y':
            trgt_axs = Y_AXIS;
            break;
          case 'z':
            trgt_axs = Z_AXIS;
            break;
        }

        float new_value = buffer.substring(15).toFloat();
        xl.setOffset(trgt_axs,new_value);
      }
    }
  }
}