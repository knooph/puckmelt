#pragma once
#include <CRSFforArduino.hpp>
#include "network_id.h"

class receiver {
  public:
  static inline uint16_t throttle;
  static inline uint16_t fb_axis;
  static inline uint16_t lr_axis;
  static inline CRSFforArduino* crsf;

  static void init();
  static void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
  static void printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels);

    static inline uint16_t channels[32];
    static inline unsigned long time;
    static inline uint32_t latency;
};

#define rc_update receiver::crsf->update