#include <Arduino.h>
#include "NimBLEDevice.h"


class bluetooth {
  public:
    bluetooth();
    void scanFor(int seconds);
    void scanForPrint(int seconds);

  private:
    NimBLEScan* scanner;
    NimBLEScanResults last_scan;
};


