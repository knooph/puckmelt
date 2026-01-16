#pragma once
#include "settings.h"
#include "Arduino.h"

class receiver {
  int fb;
  int lr;
  int th;
  int st;
  int fb_mem;
  int lr_mem;
  int st_mem;
  unsigned long time;
  public:
  receiver();
  void init();
  float getSteering();
  int getThrottle();
  int getForward();
  int getSideways();
  bool selfDestruct();
};

