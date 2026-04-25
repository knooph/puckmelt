#include "radio.h"

//initialize module
void Receiver::init() {
  
  crsf = new CRSFforArduino(&Serial1,RX,TX);
  // crsf = new CRSFforArduino(&Serial1, 0, 1);
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
  crsf->setRcChannelsCallback(Receiver::onReceive);
  crsf->setLinkStatisticsCallback(Receiver::onLinkStatisticsUpdate);
}

void Receiver::onReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  if (!rcChannels->failsafe && !watchdog_enable) { //if the builtin failsafe isn't active, update channels, otherwise set to 0
    angle = crsf->rcToUs(crsf->getChannel(ANGLE_OFFSET_CHANNEL));
    throttle = crsf->rcToUs(crsf->getChannel(THROTTLE_CHANNEL));
    fb_axis = crsf->rcToUs(crsf->getChannel(FORWARD_BACKWARD_CHANNEL));
    lr_axis = crsf->rcToUs(crsf->getChannel(LEFT_RIGHT_CHANNEL));
    angle = map(angle,1500,2012,0,2*PI);
    throttle = map(throttle,988,2012,0,100);
    fb_axis = map(fb_axis,988,2012,-100,100);
    lr_axis = map(lr_axis,988,2012,-100,100);
  } else {
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

void Receiver::onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics) {
  quality = linkStatistics.lqi;
  if (quality > 80) { //only consider quality signals to be a received signal
    last_receive_time = millis();
  }

  if (millis() - last_receive_time> 1000) { //if a quality signal hasn't been received for over 1 second, set channels to 0
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

//updates the crossfire library and checks watchdog
void Receiver::handle() {
  crsf->update();
  //redundant protocol to ensure the check is being run in the loop (crsf should be running onLinkStatisticsUpdate but just in case)
  if (millis() - last_receive_time > 1000) { //if a quality signal hasn't been received for over 1 second, set channels to 0
    fb_axis = 0;
    lr_axis = 0;
    throttle = 0;
  }
}

void Receiver::batt_telemetry(float voltage, float current, uint32_t fuel, uint8_t percent) {
  crsf->telemetryWriteBattery(voltage, current, fuel, percent);
}