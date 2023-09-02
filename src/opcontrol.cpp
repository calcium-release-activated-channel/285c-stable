#ifndef OPCONTROL_CPP
#define OPCONTROL_CPP

#include "opcontrol.h"

#include "main.h"
// #include "autonomous.h"

pros::Task buttonInterruptsTask = pros::Task(buttonInterrupts_fn, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Button Interrupt Manager");
pros::Task cataSubhandlerTask = pros::Task(cataSubhandler_fn, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Cata Subhandler");
// pros::Task intakeSubhandlerTask = pros::Task(intakeSubhandler_fn, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Intake Subhandler");

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
    taskKill();  // in case bot disconnects or we go from auton -> driver

    controller.clearLine(0);
    // battery warning
    if ((int)pros::battery::get_capacity() < 35 || controller.getBatteryLevel() < 30) {
        pros::delay(100);
        controller.rumble(".");
        pros::delay(100);
        controller.setText(0, 0, "Battery Low! ");
    }

    // define opcontrol vars and reset from auton
    wingsDeployed = false;
    wingsSolenoid.set_value(wingsDeployed);
    cataEnabled = true;
    ptoSolenoid.set_value(cataEnabled);
    runCata = false;
    // intakeMode = 0;

    // run task(s)
    buttonInterruptsTask.resume();
    cataSubhandlerTask.resume();
    // intakeSubhandlerTask.resume();

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

void buttonInterrupts_fn(void* param) {
    while (true) {
        if (cataBtn.changedToPressed()) {
            if (cataEnabled)
                runCata = !runCata;
            else {
                controller.rumble(".");
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
            if (!cataEnabled) {
                cataSubhandlerTask.suspend();
                runCata = false;
            }
            else
                cataSubhandlerTask.resume();
            ptoSolenoid.set_value(cataEnabled);
            printf("ptoBtn pressed\t cataEnabled = %s\n", cataEnabled ? "true" : "false");
            controller.setText(0, 0, cataEnabled ? "Cata Enabled " : "Cata Disabled");
        }
        // if (intakeBtn.changedToPressed()) {
        //     switch (intakeMode) {
        //         case -1:
        //             intakeMode = 1;
        //             break;
        //         case 0:
        //             intakeMode = 1;
        //             break;
        //         case 1:
        //             intakeMode = 0;
        //             break;
        //         default:
        //             intakeMode = 0;
        //             break;
        //     }
        // }
        // if (outtakeBtn.changedToPressed()) {
        //     switch (intakeMode) {
        //         case -1:
        //             intakeMode = 0;
        //             break;
        //         case 0:
        //             intakeMode = -1;
        //             break;
        //         case 1:
        //             intakeMode = -1;
        //             break;
        //         default:
        //             intakeMode = 0;
        //             break;
        //     }
        // }
        pros::delay(20);
    }
}

void cataSubhandler_fn(void* param) {
    while (runCata) {
        ptoGroup.moveVelocity(200);
        pros::delay((int)(2000.0 / 3.3));  // change this value
    }
    ptoGroup.moveVelocity(0);
}

// void intakeSubhandler_fn(void* param) {
//     while (true) {
//         switch (intakeMode) {
//             case -1:
//                 intake.moveVelocity(-600);
//                 break;
//             case 0:
//                 intake.moveVelocity(0);
//                 break;
//             case 1:
//                 intake.moveVelocity(600);
//                 break;
//             default:
//                 intake.moveVelocity(0);
//                 break;
//         }
//         pros::delay(200);
//     }
// }

#endif