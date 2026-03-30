#pragma once
#include <CRSFforArduino.hpp>
#include "network_id.h"

class receiver {
  public:
  uint16_t throttle;
  uint16_t fb_axis;
  uint16_t lr_axis;
  receiver();
  void init();

  private:
  CRSFforArduino* crsf = nullptr;
  void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
};