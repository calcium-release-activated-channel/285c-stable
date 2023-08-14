#ifndef PID_CPP
#define PID_CPP

#include "main.h"

PID::PID(double kP, double kI, double kD, double IconstrainLow, double IconstrainHigh) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
    this->IconstrainLow = IconstrainLow;
    this->IconstrainHigh = IconstrainHigh;
}

double PID::calculatePID(double new_target, double new_current) {
    uint32_t time = pros::millis();
    double target = new_target;
    double current = new_current;
    double error = target - current;
    double integral;
    if (error < IconstrainLow) { // to prevent integral windup
        integral = IconstrainLow;
    }
    else if (error > IconstrainHigh) { // to prevent integral windup
        integral = IconstrainHigh;
    }
    else {
        integral = (error == 0) ? 0 : integral + error * time;  // to prevent error overshoot (DISABLE FOR STUFF THAT NEEDS TO HOLD POSITION)
    }
    double derivative = (error - prevError) / (time - prevTime);
    double output = kP * error + kI * integral - kD * derivative;
    this->prevError = error;
    this->prevTime = time;
    return output;
}

PID::~PID() {}

#endif