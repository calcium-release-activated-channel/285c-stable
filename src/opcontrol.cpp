#ifndef OPCONTROL_CPP
#define OPCONTROL_CPP

#include "opcontrol.h"

#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    // define opcontrol vars
    wingsDeployed = false;
    // run task(s)
    pros::Task buttonInterruptsTask(buttonInterrupts, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Button Interrupt Manager");
    // drive
    while (true) {
        // drive
        drive->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY),
                                controller.getAnalog(ControllerAnalog::rightY));

        pros::delay(10);
    }
}

void buttonInterrupts(void* param) {
    while (true) {
        if (cataBtn.changedToPressed()) {
            // cata.moveVelocity(200);
        }
        if (wingsBtn.changedToPressed()) {
            wingsDeployed = !wingsDeployed;
            wingsSolenoid.set_value(wingsDeployed);
        }
        if (endgameBtn.changedToPressed()) {
            shiftSolenoid.set_value(true);
        }
        if (intakeBtn.changedToPressed()) {
            // intake.moveVelocity(200);
        }
        if (outtakeBtn.changedToPressed()) {
            // intake.moveVelocity(-200);
        }
        pros::delay(20);
    }
}

#endif