#ifndef AUTONOMOUS_H
#define AUTONOMOUS_H

#include "main.h"

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