#include "OTA.h"
#include "bluetooth.h"
#include <Arduino.h>

bluetooth* bt; //this is a pointer because BLE needs arduino runtime drivers to be set up
void handle_Serial();
void cmd_Serial(std::string cmd);
std::string input_buffer;
bool holdSerial = false;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(30000);
  OTA_init();
  bt = new bluetooth();
  bt->scan(5);

  delay(6000);

  for(int i = 0; i < bt->results->getCount(); i++) {
    Serial.print(i);
    Serial.print(") ");
    Serial.println(bt->results->getDevice(i).toString().c_str());
  }
}

void loop() {
  OTA_handle();
}

/*
void handle_Serial() {
  if (Serial.available() > 0 && !holdSerial) {
    

    char c = Serial.read(); //read input

    if ((c == '\b' || c == 127) && !input_buffer.empty()) { //if incoming char is a backspace/delete
      input_buffer.pop_back(); //delete the last char in buffer
      Serial.print("\b \b"); //move back, print space, move back

    } else if (c == '\n') { //if incoming char is a newline
      Serial.print('\n');
      cmd_Serial(input_buffer); //execute command
      input_buffer = ""; //clear buffer
    } else if (isAscii(c)) {
      input_buffer += c;
      Serial.print(c);
    }


  } else if (holdSerial && !bt->isScanning) {
    

      for (int i = 0; i < bt->results->getCount(); i++) {
        Serial.print(i);
        Serial.print(") ");
        Serial.println(bt->results->getDevice(i).toString().c_str());
      }
      Serial.flush();
      holdSerial = false;

  } else if (holdSerial && bt->isScanning && (micros() % 1000000 == 0)) {
    Serial.println("Scanning...");
  }
}

void cmd_Serial(std::string cmd) {
  Serial.print((cmd.substr(0,4) == "scan") ? "true" : "false");
  Serial.print(isDigit(cmd[5]) ? "true" : "false");

  if (cmd.substr(0,4) == "scan" && isDigit(cmd[5])) { //scan n
    holdSerial = true;
    bt->scan(10);
    Serial.println((bt->isScanning) ? "Begin Scanning..." : "Scan failed :(");
  } else if (cmd.substr(0,5).compare("init") && cmd.length() == 5) { // init
    Serial.println((BLEDevice::getInitialized()) ? "BLE is initialized" : "BLE is not initialized");
  }
}
*/