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
    } else { //everything else stored to 2 decimal points
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

/*Update state values based on sensor readings. Uses wheel velocity to keep track of angle*/
void physicState::update(float nrm_xl, float tan_xl, float z_xl, float rpm) {
    physicState::set(NRM_XL,nrm_xl); //set accelerometer readings
    physicState::set(TAN_XL,tan_xl);
    physicState::set(Z_XL, z_xl);
    physicState::set(ANG_V,rpm * 2*PI / 60 * WHEEL_RADIUS / WHEEL_DIST); //convert wheel rpm into bot s^-1
    physicState::set(ANGL, p(ANGL) + p(ANG_V) * TIME_INTVL);

    if (p(Z_XL) > FLIP_SENSITIVITY/100) { //Idle when near 0 acceleration (freefall)
        flip_factor = min(p(Z_XL)*1.1,1.00); //I played around with some more complex functions but linear is great for low idle speed
    } else if (p(Z_XL) < FLIP_SENSITIVITY/-100) {
        flip_factor = max(p(Z_XL)*1.1,-1.00); //cap the flip factors at -1 and 1
    }
}

/*Update state values based on sensor readings. Use tangent accel tp keep track of angle*/
void physicState::update(float nrm_xl, float tan_xl, float z_xl) {
    physicState::set(NRM_XL,nrm_xl); //set accelerometer readings
    physicState::set(TAN_XL,tan_xl);
    physicState::set(Z_XL, z_xl);
    physicState::set(ANG_V, sqrt(nrm_xl / XL_RADIUS)); //convert centripetal acceleration into s^-1
    physicState::set(ANGL, p(ANGL) + p(ANG_V) * TIME_INTVL + 0.5 * tan_xl * 9.8 / XL_RADIUS * pow(TIME_INTVL,2));

    if (p(Z_XL) > FLIP_SENSITIVITY/100) { //Idle when near 0 acceleration (freefall)
        flip_factor = min(p(Z_XL)*1.1,1.00); //I played around with some more complex functions but linear is great for low air speed
    } else if (p(Z_XL) < FLIP_SENSITIVITY/100) {
        flip_factor = max(p(Z_XL)*1.1,-1.00); //cap the flip factors at -1 and 1
    }
}
//return tangent wheel velocity
float physicState::motor_velocity(MOTOR_SIDE side, float v_trans, float weapon_rpm, float angl_offset) {
    float weapon_v = weapon_rpm * 2 * PI / 60 * WHEEL_DIST; //wheel tangent velocity to rotate bot at weapon rpm
    float trans_v = v_trans * cos(p(ANGL) - angl_offset);
    return flip_factor * (weapon_v + side * trans_v) ;
}

float physicState::velocity_to_throttle(float v) {
    return -1 * (-136 + sqrt(-0.3636*v+18517.37968)) / 0.1818; //inverse function of rpm(throttle) based on testing of our specific motor
}
/*----------------------------------------------------------------------------------------------*/

/*controlHandler----------------------------------------------------------------------------------------------*/
//updates the controller state
void controlHandler::in(int8_t x_in, int8_t y_in, int8_t th, int8_t of){
    y_input = y_in;
    x_input = x_in;
    throttle = th;
    offset = of;
}
//normalizes square input space (100 is max value)
float controlHandler::get_magnitude(int8_t x_in, int8_t y_in){
    float x_max = min(float(100),abs(100/tan(get_angle(x_in,y_in))));
    float y_max = min(float(100),abs(100*tan(get_angle(x_in,y_in))));
    return pow(x_in * x_in + y_in * y_in, 0.5) / pow(x_max * x_max + y_max * y_max, 0.5);
}
float controlHandler::get_angle(int8_t x_in, int8_t y_in) {
    float adjust_1 = (x_in < 0) ? PI : 0;
    float adjust_2 = atan(y_in/x_in + adjust_1 < 0) ? 2 * PI : 0;
    return atan(double(y_in)/double(x_in)) + adjust_1 + adjust_2;
}

//Converts wheeel rpm to wheel velocity
float controlHandler::rpm_to_v(float rpm) {
    return rpm * 2 * PI / 60 * WHEEL_RADIUS;
}

//convert wheel velocity to rpm
float controlHandler::v_to_rpm(float velocity) {
    return velocity / (2 * PI * WHEEL_RADIUS) * 60;
}

//returns the desired translation velocity based on throttle
float controlHandler::velocity() {
    float max_v = rpm_to_v(MAX_RPM_MTR);
    return (1 - throttle/100) * get_magnitude(x_input,y_input);
}

//returns the desired weapon rpm based on throttle
float controlHandler::weapon_rpm() {
    return throttle/100 * rpm_to_v(MAX_RPM_MTR)/WHEEL_DIST * 60 / (2*PI);
}

float controlHandler::get_offset() {
    return offset * 2 * PI /100; //convert from percentage to angle
}
/*----------------------------------------------------------------------------------------------*/