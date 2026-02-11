#include <Arduino.h>
#include "NimBLEDevice.h"


class bluetooth {
  public:
    bluetooth();
    NimBLEScanResults scanFor(int seconds);
    void printScan(NimBLEScanResults results);

  private:
    NimBLEScan* scanner;
};


