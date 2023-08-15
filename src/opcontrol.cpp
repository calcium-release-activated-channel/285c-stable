#ifndef OPCONTROL_CPP
#define OPCONTROL_CPP

#include "opcontrol.h"

#include "main.h"
// #include "autonomous.h"

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
    // battery warning
    if ((int)pros::battery::get_capacity() < 30 || controller.getBatteryLevel() < 30) {
        controller.rumble(".");
        pros::delay(100);
        controller.setText(0, 0, "Battery Low!");
    }
    // define opcontrol vars
    wingsDeployed = false;
    cataEnabled = true;
    // run task(s)
    pros::Task buttonInterruptsTask(buttonInterrupts, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Button Interrupt Manager");
    // drive
    /*
    pros::delay(10000);
    printf("running task\n");
    autonTest();
    */
    while (true) {
        // drive
        if (cataEnabled)
            drive4->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY),
                                     controller.getAnalog(ControllerAnalog::rightY));
        if (!cataEnabled)
            drive7->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY),
                                     controller.getAnalog(ControllerAnalog::rightY));
        pros::delay(20);
    }
}

void buttonInterrupts(void* param) {
    while (true) {
        if (cataBtn.changedToPressed()) {
            if (cataEnabled) {
                ptoFullGroup.moveVelocity(100);
                ptoHalfGroup.moveVelocity(200);
                pros::delay(2000);  // change this value
                ptoGroup.moveVelocity(0); // see if this causes issues
            }
            else {
                controller.setText(0, 0, "Cata Disabled");
            }
            printf("cataBtn pressed\n");
        }
        if (wingsBtn.changedToPressed()) {
            wingsDeployed = !wingsDeployed;
            wingsSolenoid.set_value(wingsDeployed);
            printf("wingsBtn pressed\twingsDeployed = %s\n", wingsDeployed ? "true" : "false");
        }
        if (ptoBtn.changedToPressed()) {
            cataEnabled = !cataEnabled;
            ptoSolenoid.set_value(cataEnabled);
            printf("ptoBtn pressed\t cataEnabled = %s\n", cataEnabled ? "true" : "false");
        }
        if (intakeBtn.changedToPressed()) {
            intake.moveVelocity(600);
            printf("intakeBtn pressed\n");
        }
        if (outtakeBtn.changedToPressed()) {
            intake.moveVelocity(-600);
            printf("outtakeBtn pressed\n");
        }
        pros::delay(20);
    }
}

#endif