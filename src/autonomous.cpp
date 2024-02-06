#ifndef AUTONOMOUS_CPP
#define AUTONOMOUS_CPP

#include "autonomous.h"

/**
 * <SC7> An Autonomous Win Point is awarded to any Alliance that has completed the
 * following tasks at the end of the Autonomous Period:
 *  a. Removed the Triball from the Alliance’s Match Load Zone that coincides with their
 *      Starting Tiles. For example, in Figure 21, the red Alliance must remove the Triball
 *      that begins in the bottom-left Match Load Zone, adjacent to Robot 1’s Starting Tiles.
 *  b. Scored at least one Alliance Triball in the Alliance’s own Goal.
 *  c. Ended the Autonomous Period with at least one Robot contacting their own Elevation Bar.
 *  d. Not violated any other rules.
 */

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

// decl tasks
// pros::Task armAutonTask = pros::Task(armAutonTask_fn, (void*)"", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Button Interrupt Manager");

void autonomous() {
    taskKill();  // in case we ever go from driver -> auton -> driver
    switch (autMode) {
        case 0:
            scoreGoal();
            break;
        case 1:
            noAuton();
            break;
        case 2:
            loadZone();
            break;
        case 3:
            loadZoneAndBar();
            break;
        case 4:
            scoreGoalAndBar();
            break;
        case 5:
            autonTest();
            break;
        default:
            noAuton();
            break;
    }
}

// No Autonomous
void noAuton() {}

// Load Zone: LEFT
void loadZone() {
    autonChassis.setPose(-36,-60,360-45);
    autonChassis.moveTo(-60,-36,1000);
    autonChassis.turnTo(-60,0,1000);
    autonChassis.moveTo(-60,-30,1000);
    autonChassis.moveTo(-60,-36,1000);
    // need to move near load bar
    // and expand wings
    // then remove ball by moving next to it
    // then retract wings
    // then move to elev bar
    // then expand wings to touch bar
}

// Score Goal: RIGHT
void scoreGoal() {
    autonChassis.setPose(36,-60,0);
    autonChassis.moveTo(36,-24,1000); // might need to slow down
    pros::delay(500);
    autonChassis.moveTo(36,-36,1000);
    pros::delay(500);
    autonChassis.turnTo(24,-24,1000);
    pros::delay(500);
    autonChassis.moveTo(24,-24,1000);
    wingsSolenoid.set_value(true);
    pros::delay(500);
    autonChassis.turnTo(24,0,1000);
    pros::delay(500);
    autonChassis.moveTo(24,-12,1000);
    pros::delay(500);
    autonChassis.turnTo(60,-12,1000);
    pros::delay(500);
    autonChassis.moveTo(45,-12,1000);

}

// Load Zone + Bar: LEFT+
void loadZoneAndBar() {
}

// Score Goal + Bar: RIGHT+
void scoreGoalAndBar() {
    scoreGoal();
    wingsSolenoid.set_value(false);
    autonChassis.moveTo(36,-12,1000);
    pros::delay(500);
    autonChassis.turnTo(36,-60,1000);
    pros::delay(500);
    autonChassis.moveTo(36,-60,1000);
    pros::delay(500);
    autonChassis.turnTo(10,-60,1000);
    pros::delay(500);
    autonChassis.moveTo(10,-60,1000);
    wingsSolenoid.set_value(true);
}

/**
 * @brief Synchronous drive function that corrects for differences in friction
 * @param target The target voltage to drive at, [-12000, 12000]
 */
void driveStraight(int targetL, int targetR) {  // adjust for differences in friction
    int leftAdj = 1,
        rightAdj = 1;
    driveL.moveVoltage((int)(targetL * leftAdj));
    driveR.moveVoltage((int)(targetR * rightAdj));
}

void autonTest() { // to tune PID
    // begin auton dev
    scoreGoal();
    // end auton dev
}

#endif
