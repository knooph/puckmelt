#include <Arduino.h>
#include <SPI.h>
#include "SparkFun_LIS331.h"

#define MISO_PIN GPIO_NUM_10
#define MOSI_PIN GPIO_NUM_8
#define SS_PIN GPIO_NUM_11
#define SCK_PIN GPIO_NUM_9
#define INT1_PIN GPIO_NUM_12

enum AXIS {
  X_AXIS = 0,
  Y_AXIS = 1,
  Z_AXIS = 2
};

class Accelerometer{
  LIS331 x1;
  int16_t scale1000[3];
  int16_t offset1000[3];
  int16_t sensor[3];
  public:
  void init();
  void update();
  float adjustedAccel(AXIS axis);
  float rawAccel(AXIS axis);
  void setScale(AXIS axis, float scale);
  void setOffset(AXIS axis, float offset);

  friend void handle_terminal();
};