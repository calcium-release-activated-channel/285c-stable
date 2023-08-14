#ifndef PID_CPP
#define PID_CPP

#include "main.h"

PID::PID(double kP, double kI, double kD, double Ibound) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
    this->Ibound = Ibound;
}

void PID::setTarget(double new_target) {
    this->target = new_target;
}

double PID::calculatePID(double new_current) {
    uint32_t time = pros::millis();
    double current = new_current;
    double error = this->target - current;
    double integral = integral + error * time;
    integral = (error == 0 || abs(integral) > Ibound) ? 0 : integral;
    // ibound to filter absurdly large/useless integral values
    // error=0 to prevent error overshoot (DISABLE FOR STUFF THAT NEEDS TO HOLD POSITION)
    double derivative = (error - prevError) / (time - prevTime);
    double output = kP * error + kI * integral - kD * derivative;
    this->prevError = error;
    this->prevTime = time;
    return output;
}

PID::~PID() {}

#endif