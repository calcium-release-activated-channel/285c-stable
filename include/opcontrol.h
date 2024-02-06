#ifndef OPCONTROL_H_
#define OPCONTROL_H_

#include "main.h"

#include "autonomous.h" // auton tests

// declare opcontrol vars
bool wingsDeployed; // whether wings are deployed

void buttonInterrupts_fn(void* param);

#endif