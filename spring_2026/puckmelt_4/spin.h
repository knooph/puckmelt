#include "settings.h"

enum SIDE {
    LEFT = -1,
    RIGHT = 1
};

class spin {
    public:
    spin();
    static inline float angle;
    static inline float angle_step;
    static inline float max_speed;
    static float wheel_speed(SIDE side, uint8_t throttle_percent);
    static void step(float angle_inject);
};

float speed_to_throttle(float speed);