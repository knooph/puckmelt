#include "radio.h"
#include "wireless.h"
#include "rotation.h"
#include "accel.h"

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
controlHandler controller; //Converts driver input into useful information for the physicState
physicState puckmath; //does the real-time rotational kinematics using information from the controller. Read target motor throttle off of this object
Accelerometer xl = Accelerometer();

TaskHandle_t Task0;// Everything on core 0
TaskHandle_t Task1;//Everything on core 1

void handle_terminal();//forward declaration

void setup() {
  Serial.begin(115200); //Don't actually need this, but leaving it here for debug output

  //Initialize modules
  laptop.init();
  radio.init();
  xl.init();


  xTaskCreatePinnedToCore(loop0, "Task0", 10000, NULL, 1, &Task0, 0);
  delay(500);
  xTaskCreatePinnedToCore(loop1, "Task1", 10000, NULL, 1, &Task1, 1);

  Serial.end();
  laptop.print(graphic);
}

void loop() {}

//core0 will be dedicated to the tasks that need to be exxecuted in real time like sensor input and motor output.
void loop0(void* pvParameters) {
  radio.handle();
  xl.update();
  puckmath.update(xl.adjustedAccel(Y_AXIS),xl.adjustedAccel(X_AXIS),xl.adjustedAccel(Z_AXIS)); //   <<Y AXIS, X AXIS, Z AXIS

  puckmath.motor_throttle(RIGHT, controller.velocity(), controller.weapon_rpm(), controller.get_offset());// How to getRight motor throttle
  puckmath.motor_throttle(LEFT, controller.velocity(), controller.weapon_rpm(), controller.get_offset());// How to get left motor throttle
}

void loop1(void* pvParameters) {
  laptop.handle();
  radio.batt_telemetry(0.0,0.0,0,0);//  <<<<BATTERY TELEMETRY HERE
  controller.in(radio.lr_axis, radio.fb_axis, radio.throttle, radio.angle);
  send_data(xl.adjustedAccel(Y_AXIS),xl.adjustedAccel(X_AXIS),xl.adjustedAccel(Z_AXIS));
  handle_terminal();
}



void send_data(float nrml_xl, float tan_xl, float z_xl) {
  String csv_line = String(nrml_xl) + "," +String(tan_xl) + "," + String(z_xl);
  laptop.println(csv_line);
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
  // END EXECUTE LOGIC
  //END TERMINAL FUNCTION
}