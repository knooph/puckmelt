#include "rc.h"
#include "network_id.h"

void onReceive(serialReceiverLayer::rcChannels_t *rcChannels);
void printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels);

void receiver::init() {
  crsf = new CRSFforArduino(&Serial0,3,1);
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

void receiver::init(WiFiClient* serial) {
  crsf = new CRSFforArduino(&Serial0,3,1);
  debug_out = serial;
  while(true) { //hold program hostage if crsf fails
    if(crsf->begin()){
      crsf->setRcChannelsCallback(receiver::printOnReceive);
      crsf->setLinkStatisticsCallback(receiver::onLinkStatisticsUpdate);
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

// void receiver::onReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
//   throttle = crsf->rcToUs(crsf->getChannel(TH_CHANNEL));
//   fb_axis = crsf->rcToUs(crsf->getChannel(FB_CHANNEL));
//   lr_axis = crsf->rcToUs(crsf->getChannel(LR_CHANNEL));
//   latency = millis() - time;
//   time = millis();
// }

void receiver::printOnReceive(serialReceiverLayer::rcChannels_t *rcChannels) {
  int count = crsfProtocol::RC_CHANNEL_COUNT;
  for (int i = 1; i < count+1; i++) {
    channels[i] = crsf->rcToUs(crsf->getChannel(i));
  }
  latency = millis() - time;
  time = millis();
}

void receiver::onLinkStatisticsUpdate(serialReceiverLayer::link_statistics_t linkStatistics) {
    /* This is your Link Statistics Event Callback.
    By using the linkStatistics parameter that's passed in,
    you have access to the following:
    - linkStatistics.rssi
    - linkStatistics.lqi
    - linkStatistics.snr
    - linkStatistics.tx_power

    For the purposes of this example, these values are simply
    printed to the Serial Monitor at a rate of 5 Hz. */

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 200)
    {
        lastPrint = millis();
        debug_out->print("Link Statistics: ");
        debug_out->print("RSSI: ");
        debug_out->print(linkStatistics.rssi);
        debug_out->print(", Link Quality: ");
        debug_out->print(linkStatistics.lqi);
        debug_out->print(", Signal-to-Noise Ratio: ");
        debug_out->print(linkStatistics.snr);
        debug_out->print(", Transmitter Power: ");
        debug_out->println(linkStatistics.tx_power);
    }
}