#ifndef OPCONTROL_CPP
#define OPCONTROL_CPP

#include "opcontrol.h"

pros::Task buttonInterruptsTask = pros::Task(buttonInterrupts_fn, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Button Interrupt Manager");

void autonomous() {
    taskKill();
}

void opcontrol() {
    taskKill();  // in case bot disconnects or we go from auton -> driver

    controller.clearLine(0);
    // battery warning
    if ((int)pros::battery::get_capacity() < 35 || controller.getBatteryLevel() < 30) {
        pros::delay(100);
        controller.rumble(".");
        pros::delay(100);
        controller.setText(0, 0, "Battery Low! ");
    }

    // reset from auton
    clampState = false;
    clampSolenoid.set_value(clampState);

    // run task(s)
    buttonInterruptsTask.resume();

    while (true) { 
        driveL.moveVoltage((int)(12000 * controller.getAnalog(ControllerAnalog::leftY)));
        driveR.moveVoltage((int)(12000 * controller.getAnalog(ControllerAnalog::rightY)));
        pros::delay(20);
    }
}

void buttonInterrupts_fn(void* param) {
    while (true) {
        if (intakeBtn.changedToReleased() || outtakeBtn.changedToReleased()) {
            intake.moveVelocity(0);
            conveyor.moveVelocity(0);
        }
        if (intakeBtn.isPressed() && !outtakeBtn.isPressed()) {
            intake.moveVelocity(600);
            conveyor.moveVelocity(450);
        }
        if (outtakeBtn.isPressed() && !intakeBtn.isPressed()) {
            intake.moveVelocity(-600);
            conveyor.moveVelocity(-450);
        }
        if (clampBtn.changedToPressed()) {
            clampState = !clampState;
            clampSolenoid.set_value(clampState);
        }
        pros::delay(20);
    }
}

#endif
