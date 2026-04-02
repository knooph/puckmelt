#include "rotation.h"

#define p physicState::get

/*physicState----------------------------------------------------------------------------------------------*/

/*Return a specific value from the current state defined by sensor readings 
3 accelerometer values, 1 motor telemetry, tracked theta*/
float physicState::get(PHYS_VAL index) {
    if (index == ANGL) { //heading stored so error is less than 0.0001 for 0 to 2 pi
        return (physicState::state[ANGL] + 32768) / 10430;
    } else if (index == ANG_V) { //
        return physicState::state[ANG_V] / 10.5; //angular velocity stored so errors is less than 0.0875 s^-1 
    } else { //accelerations stored to 2 decimal points
        return physicState::state[index] / 100; //-327.68, 327.67 max values
    }
}
/*Write a specific value to the current state*/
void physicState::set(PHYS_VAL index, float value) {
    if (index == ANGL) {
        physicState::state[ANGL] = (value * 10430 - 32768 > 32765) ? value * 10430 - 65533: value * 10430 - 32768; //loops the angle if it overflows
    } else if (index == ANG_V) {
        physicState::state[ANG_V] = value * 10.5;
    } else {
        physicState::state[index] = value * 100;
    }
}

/*Calculate the linear x acceleration (bot east)*/
float physicState::x_xl() {
    return (p(NRM_XL) + pow(p(ANG_V),2) * WHEEL_RADIUS) * cos(p(ANGL)) + p(TAN_XL) * sin(p(ANGL));
            // accelerometer reading + the calculated centripetal acceleration (because it is inward therefore negative) cosine + the sin of the tangent accelerometer reading
}

/*Calculate the linear y acceleration (bot north)*/
float physicState::y_xl() {
    return (p(NRM_XL) + pow(p(ANG_V),2) * WHEEL_RADIUS) * sin(p(ANGL)) + p(TAN_XL) * sin(p(ANGL));
}

void physicState::update(float nrm_xl, float tan_xl, float z_xl, float rpm) {
    physicState::set(NRM_XL,nrm_xl); //set accelerometer readings
    physicState::set(TAN_XL,tan_xl);
    physicState::set(Z_XL,z_xl);
    physicState::set(ANG_V,rpm * 6.28318530718 / 60 * WHEEL_RADIUS / WHEEL_DIST); //convert wheel rpm into bot s^-1
    physicState::set(ANGL, p(ANGL) + p(ANG_V) * TIME_INTVL);
}
/*----------------------------------------------------------------------------------------------*/

/*controlHandle----------------------------------------------------------------------------------------------*/
CTRL_MODE controlHandle::get_mode() {}
void controlHandle::set_mode(CTRL_MODE ctrl_mode) {}
float controlHandle::motor_rpm() {}
/*----------------------------------------------------------------------------------------------*/