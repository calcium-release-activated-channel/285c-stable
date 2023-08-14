#ifndef PID_CPP
#define PID_CPP

#include "main.h"

PID::PID(double kP, double kI, double kD, double Ibound, double outBound) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
    this->Ibound = Ibound;
    this->outBound = outBound;
}

void PID::setTarget(double new_target) {
    this->target = new_target;
}

double PID::calculatePID(double new_current) {
    uint32_t time = pros::millis();
    double current = new_current;
    double error = this->target - current;
    integral += error * time;
    // printf("raw integral: %f\n", integral); // debug
    integral = (error == 0 || abs(integral) > Ibound) ? 0 : integral;
    // ibound to filter absurdly large/useless integral values
    // error=0 to prevent error overshoot (DISABLE FOR STUFF THAT NEEDS TO HOLD POSITION)
    double derivative = (error - prevError) / (time - prevTime);
    double output = kP * error + kI * integral - kD * derivative;
    // debug
    // printf("using value kP = %f, kI = %f, kD = %f, Ibound = %f\n", kP, kI, kD, Ibound);
    // printf("error: %f\t | integral: %f\t | derivative: %f | output: %f\n", error, integral, derivative, output);
    this->prevError = error;
    this->prevTime = time;
    return (outBound == 0) ? output : (abs(output) > outBound) ? outBound * copysign(1.0, output) : output;
}

PID::~PID() {}

#endif