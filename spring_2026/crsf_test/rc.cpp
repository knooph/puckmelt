#include "rc.h"
#include "network_id.h"

void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
void printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels);

void receiver::init() {
  crsf = new CRSFforArduino(&Serial0,1,2);
  while(true) { //hold program hostage if crsf fails
    if(crsf->begin()){
      crsf->setRcChannelsCallback(receiver::printOnReceive);
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
  latency = millis() - time;
  time = millis();
}

void receiver::printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  int count = crsfProtocol::RC_CHANNEL_COUNT;
  for (int i = 1; i < count+1; i++) {
    channels[i] = crsf->rcToUs(crsf->getChannel(i));
  }
  latency = millis() - time;
  time = millis();
}