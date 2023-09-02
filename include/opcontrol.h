#ifndef OPCONTROL_H_
#define OPCONTROL_H_

#include "main.h"

// declare opcontrol vars
bool wingsDeployed; // whether wings are deployed
bool cataEnabled; // true: pto on cata, false: pto on drive
bool runCata; // whether cata is running
// int intakeMode; // -1 out 0 off 1 in

void buttonInterrupts_fn(void* param);
void cataSubhandler_fn(void* param);
// void intakeSubhandler_fn(void* param);

#endif