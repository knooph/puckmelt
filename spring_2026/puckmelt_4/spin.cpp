#include "spin.h"
#include <math.h>
spin::spin(){
    angle = 0;
    angle_step = PI/360;
}

float spin::wheel_speed(SIDE side, uint8_t throttle_percent) {
    return (throttle_percent*max_speed/100) + side * ((1-throttle_percent)*max_speed/100)*cos(angle);
}

void spin::step(float angle_inject) {
    angle += angle_step + angle_inject;
    if (angle > 2 * PI){
        angle = fmodf(angle, 2 * PI);
    }
}

float speed_to_throttle(float speed) {
    return speed; //     <<Put inverse throttle function here.
}