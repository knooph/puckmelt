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
}

void loop() {
  OTA_handle();
  handle_Serial();
}

void handle_Serial() {
  if (Serial.available() > 0 && !holdSerial) { //Generic Serial input
    char c = Serial.read();
    input_buffer += c;
    Serial.print(c);
    if (c == '\n') {
      Serial.println("cmd detected");
      input_buffer.pop_back();
      Serial.println(input_buffer.c_str());
      cmd_Serial(input_buffer);
      input_buffer = "";
    } else if (c == '\b') {
      input_buffer.pop_back();
      Serial.print(" ");
      input_buffer.pop_back();
    }
  } else if (holdSerial && Serial.peek() == 1 && !bt->isScanning) { //Hold serial while scanning
      for (int i = 0; i < bt->results->getCount(); i++) {
        Serial.print(i);
        Serial.print(") ");
        Serial.print(bt->results->getDevice(i).toString().c_str());
      Serial.flush();
      holdSerial = false;
    }
  }
}

void cmd_Serial(std::string cmd) {
  // Serial.println(cmd.substr(0,5).c_str());
  // Serial.println(cmd.substr(5).c_str());
  // for (int i = 0; i < String(cmd.substr(5).c_str()).toInt(); i++) {
  //   Serial.println(cmd.substr(0,5).c_str());
  // }
  if (cmd.substr(0,5).compare("scan") && !cmd.substr(5).empty()) {
    holdSerial = true;
    //If cmd is "scan number", convert number to c_str to build arduino String to convert to Integer
    Serial.print("Scanning for ");
    Serial.print(String(cmd.substr(5).c_str() ).toInt());
    Serial.println(" seconds.");
    bt->scan( String( cmd.substr(5).c_str() ).toInt() );
    Serial.println( (bt->isScanning) ? "true" : "false");
  }
}