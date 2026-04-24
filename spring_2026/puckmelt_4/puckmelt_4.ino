#include "radio.h"
#include "wireless.h"
#include "spin.h"
#include "motor.h"

/*
This is the backup program if puckmelt_3 is not completed by comp

CONTROLS:
left stick up/down, forward backward
left stick right/left like a secondary throttle kind of? It multiples the forward backward drive by 1/-1 (it's flip factor)
right stick heading is a fraction of angular velocity
*/

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
puckMotor left = puckMotor(LEFT_MOTOR_PIN); //initialize motors
puckMotor right = puckMotor(RIGHT_MOTOR_PIN);

uint8_t yaw_divisor = 50;
int8_t flip_factor = 1;

void handle_terminal();//forward declaration

float remap_angle(float angle) {
  if (angle <= 3*PI/2) {
    return angle - PI/2;
  } else {
    return angle - 2 * PI;
  }
}

void setup() {
  Serial.begin(115200); //Don't actually need this, but leaving it here for debug output

  //Initialize modules
  laptop.init();
  radio.init();
  Serial.end();
  pinMode(5,OUTPUT);
}

void loop() {
  radio.handle();
  laptop.handle();
  spin::step(-1 * (remap_angle(radio.angle)) / yaw_divisor);
  flip_factor = radio.lr_axis / 100;

  if (spin::angle < PI / 16 && spin::angle > -1* PI / 16) {
    digitalWrite(5,HIGH);
  }
  else {
    digitalWrite(5,LOW);
  }
  

  right.throttle(flip_factor * speed_to_throttle( spin::wheel_speed(RIGHT,radio.fb_axis)));
  left.throttle(flip_factor * speed_to_throttle( spin::wheel_speed(LEFT,radio.fb_axis)));

  radio.batt_telemetry(0.0,0.0,0,0);
  laptop.print("");
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
      laptop.println("KILLING ROBOT");
      radio.watchdog_enable = true;//just set everything to 0
      radio.throttle = 0;
      radio.fb_axis = 0;
      radio.lr_axis = 0;
      right.shutdown();
      left.shutdown();
    } else if (buffer.compareTo("restart") == 0) { // "restart"
      laptop.println("Restarting...");
      ESP.restart();//restart the entire program, only way to un-kill
    } else if (buffer.startsWith("spin ")) {
      int throttle_input = buffer.substring(5,buffer.indexOf(',')).toInt();
      int delay_input = buffer.substring(buffer.indexOf(',')+1).toInt();
      laptop.println("Command receieved: throttle set to: " + String(throttle_input) + " for " + String(delay_input) + " seconds");
      right.throttle(throttle_input);
      left.throttle(throttle_input);
      delay(1000*delay_input);
      right.throttle(0);
      left.throttle(0);
    } else if (buffer.startsWith("yaw ")) {
      yaw_divisor = buffer.substring(4).toInt();
      laptop.println("Yaw divisor set to " + String(yaw_divisor));
    }
  }
  // END EXECUTE LOGIC
  //END TERMINAL FUNCTION
}