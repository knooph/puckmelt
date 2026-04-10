#include "radio.h"
#include "wireless.h"
#include "rotation.h"

#include <string.h>

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
controlHandler controller; //Converts driver input into useful information for the physicState
physicState puckmath; //does the real-time rotational kinematics using information from the controller. Read target motor throttle off of this object

TaskHandle_t Task0;// Everything on core 0
TaskHandle_t Task1;//Everything on core 1

void handle_terminal();//forward declaration

void setup() {
  Serial.begin(115200); //Don't actually need this, but leaving it here for debug output

  //Initialize modules
  laptop.init();
  radio.init();


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
  puckmath.update(0.0,0.0,0.0,0.0); //   <<Y AXIS, X AXIS, Z AXIS, MOTOR RPM
  puckmath.motor_throttle(RIGHT, controller.velocity(), controller.weapon_rpm(), controller.get_offset());// How to getRight motor throttle
  puckmath.motor_throttle(LEFT, controller.velocity(), controller.weapon_rpm(), controller.get_offset());// How to get left motor throttle
}

void loop1(void* pvParameters) {
  laptop.handle();
  controller.in(radio.lr_axis, radio.fb_axis, radio.throttle, radio.angle);
  handle_terminal();
}


enum ANSI_ESC {
    CLEAR_SCREEN = '\e[2J',
    CLEAR_LINE = '\e[2K',
    BACKSPACE = '\b',
    MOVE_DOWN =  '\e[1B',
    MOVE_UP = '\e[1A',
    MOVE_RIGHT = '\e[1C',
    MOVE_LEFT = '\e[1D',
    SCREEN_MODE = '\e[=0h'
};

//This is command logic for the PuTTy terminal
void handle_terminal() {
  if (!laptop.WiFiEnabled || !laptop.client.connected()) { return; } //quit if wifi is not enabled or client isnt connected

  WiFiClient* out = &laptop.client;
  String buffer;
  bool execute = false;
  int end = buffer.length() - 1;

  if (out->available() > 0) { //read into the buffer
    buffer += out->readString();
  }

  //string cleaning and cursor logic
  if (buffer.charAt(end) == '\b') { //backspace handling
    if (end >= 1){
      buffer.remove(end - 1, 2);
    } else {
      buffer.clear();
    }
  } else if (buffer.charAt(end) == '\n' || buffer.charAt(end) == '\r') { //don't execute until user presses enter
    buffer.trim();
    execute = true;
  }
  out->print("\e[29;1\e[0K");
  out->print(buffer);
  //actual commands
  if (execute) {
    if (buffer.compareTo("kill") == 0) { //just set everything to 0
      radio.watchdog_enable = true;
      radio.throttle = 0;
      radio.fb_axis = 0;
      radio.lr_axis = 0;
      controller.throttle = 0;
      controller.x_input = 0;
      controller.y_input = 0;
    } else if (buffer.compareTo("restart") == 0) { //restart the entire program, only way to un-kill
      ESP.restart();
    } else if (buffer.startsWith("configure ")) { //change a value during runtime.

      if (buffer.substring(10).startsWith("maxrpm ")) {
        uint new_value = buffer.substring(10).substring(7).toInt();
        SETTINGS_ACCESS::max_rpm(new_value);
      } else if (buffer.substring(10).startsWith("timeint ")) {
        double new_value = buffer.substring(10).substring(8).toDouble();
        SETTINGS_ACCESS::time_int(new_value);
      } else if (buffer.substring(10).startsWith("flipsens ")) {
        uint8_t new_value = buffer.substring(10).substring(9).toInt();
        SETTINGS_ACCESS::flip_sens(new_value);
      }

    } else if (buffer.compareTo("refresh") == 0) {
      out->print("\e[H");
      out->print(graphic);
    }
  }
}