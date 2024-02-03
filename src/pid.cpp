#ifndef PID_CPP
#define PID_CPP

#include "pid.h"

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
    if (kI != 0) {
        integral += error;
        // printf("raw integral: %.3f\n", integral); // debug
        integral = (error == 0 || abs(integral) > Ibound) ? 0 : integral;
        // ibound to filter absurdly large/useless integral values
        // error=0 to prevent error overshoot (DISABLE FOR STUFF THAT NEEDS TO HOLD POSITION)
    }
    else
        integral = 0;
    double derivative = (error - prevError) / (time - prevTime);
    double output = kP * error + kI * integral - kD * derivative;
    // debug
    // printf("using value kP = %.3f, kI = %.3f, kD = %.3f, Ibound = %.3f\n", kP, kI, kD, Ibound);
    printf("error: %.3f\t | integral: %.3f\t | derivative: %.3f | output: %.3f\n", error, integral, derivative, output);
    // printf("%f,%f", time, new_current);
    this->prevError = error;
    this->prevTime = time;
    return (outBound == 0) ? output : (output > outBound) ? outBound * copysign(1.0, output)
                                                          : output;
}

PID::~PID() {}

#endif