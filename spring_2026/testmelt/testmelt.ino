#include "radio.h"
#include "wireless.h"
#include "rotation.h"
#include "accel.h"
#include "motor.h"

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
controlHandler controller; //Converts driver input into useful information for the physicState
physicState puckmath; //does the real-time rotational kinematics using information from the controller. Read target motor throttle off of this object
Accelerometer xl = Accelerometer();

puckMotor rmotor = puckMotor(RIGHT_MOTOR_PIN);
puckMotor lmotor = puckMotor(LEFT_MOTOR_PIN);

String debug = "";

TaskHandle_t main_loop;

void handle_terminal();//forward declaration

void setup() {
  Serial.begin(115200); //Don't actually need this, but leaving it here for debug output

  //Initialize modules
  xl.init();
  delay(10);
  laptop.init();
  delay(10);
  radio.init();

  rmotor.init();
  lmotor.init();

  Serial.println("Enter Test number into puckmelt_terminal");
  laptop.println("Enter Test number into puckmelt_terminal");

  while (!laptop.available()) {
      laptop.handle();
      delay(10);
    }
  int test_case = laptop.read().toInt();
  debug = String(test_case);
  Serial.println(test_case);

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
      xTaskCreatePinnedToCore(accel_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
    case 5:
      xTaskCreatePinnedToCore(rotation_loop,"Task", 10000, NULL, 1, &main_loop, 0);
      break;
  }
}

void loop() {}

void wireless_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    handle_terminal();

    send_data(100, 100, PI, 50, random(-1,1),0,2,debug);
    
    delay(100);
  }
  vTaskDelete(NULL); //if the task ends delete it
}

void radio_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.handle();
    handle_terminal();

    radio.batt_telemetry(1,2,3,4);
    send_data(radio.fb_axis, radio.lr_axis, radio.angle, radio.throttle, random(-1,1),0,2,debug);
    delay(10);
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
    unsigned long time = millis();
    if (time < 10000) {
      r = 10;
      l = 10;
    } else if (time < 20000) {
      r = 20;
      l = 20;
    } else if (time < 30000) {
      r = 30;
      l = 30;
    } else if (time < 40000) {
      r = 40;
      l = 40;
    } else if (time < 50000) {
      r = 50;
      l = 50;
    } else if (time < 60000) {
      r = 60;
      l = 60;
    } else if (time < 70000) {
      r = 70;
      l = 70;
    } else if (time < 80000) {
      r = 80;
      l = 80;
    } else if (millis() < 90000) {
      r = 90;
      l = 90;
    } else if (millis() < 100000) {
      r = 100;
      l = 100;
    }
    rmotor.throttle(r);
    lmotor.throttle(l);
    radio.batt_telemetry(1,2,3,4);
    // send_data(radio.fb_axis, radio.lr_axis, radio.angle, radio.throttle, xl.adjustedAccel(Y_AXIS),xl.adjustedAccel(X_AXIS),xl.adjustedAccel(Z_AXIS),debug);
    send_data(1, 1, 1, 1, l,r,0,debug);
    delay(10);
  }
  vTaskDelete(NULL); //if the task ends delete it
}

void accel_loop(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.handle();
    handle_terminal();
    xl.update();

    int v = 0;
    if (debug.toFloat() != 0) {
      v = sqrt(debug.toFloat() * 9.8 / XL_RADIUS) * WHEEL_DIST; //convert specific centripetal acceleration into wheel velocity
    } else if (debug.compareTo("sin")) {
      v = 20 * sin(millis() * PI / 10); //sets wheel velocity to the sine wave of v = 20 sin(pi/20 t). This means our tangential acceleration should be pi * cos(pi/20 t);
    }

    rmotor.throttle(puckmath.velocity_to_throttle(v));
    lmotor.throttle(puckmath.velocity_to_throttle(v));

    float data_1 = (debug.toFloat() != 0) ? xl.adjustedAccel(Y_AXIS) : PI * cos(millis() * PI / 10); //if we're in const accel mode show centripetal accel, otherwise the target tangential accel
    float data_2 = (debug.toFloat() != 0) ? debug.toFloat() : xl.adjustedAccel(X_AXIS); //if we're in const accel mode show target accel, otherwise show tangential accel

    radio.batt_telemetry(1,2,3,4);
    send_data(radio.fb_axis, radio.lr_axis, radio.angle, radio.throttle, data_1, data_2, xl.adjustedAccel(Z_AXIS),debug);
    delay(10);
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
  while (out->peek() != -1) {
    buffer += char(out->read());
  }
  int end = buffer.length() - 1;
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
      debug = "Program killed";
    } else if (buffer.compareTo("restart") == 0) { // "restart"
      debug = "restarting...";
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
      debug = "configured";
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
      //debug = "accelerometer configured";
    } else if (buffer.startsWith("echo ")) {
      debug = buffer.substring(5);
    }
  }

}

void send_data(uint16_t fb, uint16_t lr, uint16_t an, uint16_t th, float ct, float tn, float z, String py_log) {
  laptop.println(String(fb) + ", " + String(lr) + ", "  + String(an) + ", " + String(th) + ", " + String(ct) + ", " + String(tn) + ", " + String(z) + ", " + py_log);
}