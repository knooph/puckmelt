#include "rc.h"
#include "network_id.h"

receiver::receiver() {}

void receiver::init() {
  crsf = new CRSFforArduino();
  while(true) { //hold program hostage if crsf fails
    if(crsf->begin()){
      break;
    } else {
      Serial.println("crsf failed");
      crsf->end();
      delete crsf;
      crsf = new CRSFforArduino();
      delay(10);
    }
  }
  
}

void receiver::onReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  throttle = crsf->rcToUs(crsf->getChannel(TH_CHANNEL));
  fb_axis = crsf->rcToUs(crsf->getChannel(FB_CHANNEL));
  lr_axis = crsf->rcToUs(crsf->getChannel(LR_CHANNEL));
}