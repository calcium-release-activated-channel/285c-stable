#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include "main.h"

#define PID_AUT_OPT kP, kI, kD, Ibound, outBound
#define PID_AUT_DRV autonL, autonR

void noAuton();
void elevationBar();
void loadZone();
void scoreGoal();
void loadZoneAndBar();
void scoreGoalAndBar();
void autonTest();
void driveStraight(int target);
void PIDdriveStraight(int target, PID PIDcontrollerL, PID PIDcontrollerR);

#endif