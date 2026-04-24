#include "radio.h"
#include "wireless.h"

Wireless laptop = Wireless(); //WiFi interface witht the robot.
Receiver radio; //Represents actual receiver on the robot. Read user input off this object
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
}

void loop() {}

//core0 will be dedicated to the tasks that need to be exxecuted in real time like sensor input and motor output.
void loop0(void* pvParameters) {
  for (;;) {
    radio.handle();
  }
  vTaskDelete(NULL); //if the task ends delete it
}

void loop1(void* pvParameters) {
  for (;;) {
    laptop.handle();
    radio.batt_telemetry(0.0,0.0,0,0);//  <<<<BATTERY TELEMETRY HERE
    handle_terminal();
  }
  vTaskDelete(NULL); //if the task ends delete it
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

    } else if (buffer.compareTo("restart") == 0) { // "restart"
      ESP.restart();//restart the entire program, only way to un-kill

    } else if (buffer.startsWith("configure ")) {
      
    }
  }
  // END EXECUTE LOGIC
  //END TERMINAL FUNCTION
}