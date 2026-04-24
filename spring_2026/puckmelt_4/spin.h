#include "settings.h"

enum SIDE {
    LEFT = -1,
    RIGHT = 1
};

class spin {
    public:
    spin();
    static float angle;
    static float angle_step;
    static uint8_t base_speed;
    static float wheel_speed(SIDE side, float angle_inject);
    static void set_step(float new_step);
    static void step();
};