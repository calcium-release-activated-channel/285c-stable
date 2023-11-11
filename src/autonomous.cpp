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
            noAuton();
            break;
        case 1:
            elevationBar();
            break;
        case 2:
            loadZone();
            break;
        case 3:
            scoreGoal();
            break;
        case 4:
            loadZoneAndBar();
            break;
        case 5:
            scoreGoalAndBar();
            break;
        case 6:
            autonTest();
            break;
        default:
            noAuton();
            break;
    }
}

// No Autonomous
void noAuton() {}

// Elevation Bar: +
void elevationBar() {}

// Load Zone: LEFT
void loadZone() {
    // driveL.moveVoltage(4000);
    // driveR.moveVoltage(4000);
    // pros::delay(250);
    // driveL.moveVoltage(0);
    // driveR.moveVoltage(0);

    drive4->getModel()->tank(200, 200);
    pros::delay(1000);
    drive4->getModel()->tank(0, 0);

    // drive4->moveDistance(-24_in); //back robot out of starting tile
    // drive4->turnAngle(45_deg); //turn robot to face load zone
    // drive4->moveDistance(34_in); //move robot to load zone or 24*sqrt(2)
    // intake.moveVelocity(100); //start intake
    // pros::delay(2800);
    // intake.moveVelocity(0); //stop intake
    // drive4->moveDistance(-14_in); //move robot back to intersection of tile
    // intake.moveVelocity(-100);     // start intake
    // pros::delay(2800);
    // intake.moveVelocity(0);  // stop intake
    // drive4->turnAngle(180_deg);
    // drive4->moveDistance(-14_in);
}

// Score Goal: RIGHT
void scoreGoal() {
    // forward 2 tile
    driveStraight(6000, 6000);
    pros::delay(1500);
    driveStraight(0, 0);
    pros::delay(20);

    // back 1 tile
    driveStraight(-6000, -6000);
    pros::delay(700);
    driveStraight(0, 0);
    pros::delay(20);

    // // forward 1 tile
    // driveStraight(6000, 6000);
    // pros::delay(500);
    // driveStraight(0, 0);
    // pros::delay(20);

    // 45 degrees left
    driveStraight(-6000, 6000);
    pros::delay(250);  // 500 for a 90 deg turn
    driveStraight(0, 0);
    pros::delay(20);

    // move triangle distance
    driveStraight(3000, 3000);
    pros::delay(1500);  // 750 / 3 * sqrt(2)
    driveStraight(0, 0);
    pros::delay(20);

    // 45 degrees right
    driveStraight(3000, -3000);
    pros::delay(500);
    driveStraight(0, 0);
    pros::delay(20);

    // // arc motion right until have turned 45
    // driveStraight(3000, 0);
    // pros::delay(800);  // modify this value experimentally
    // driveStraight(0, 0);
    // pros::delay(20);

    // // arc motion right until have turned 45
    // driveStraight(3000, 0);
    // pros::delay(800);  // modify this value experimentally; 350 for 3000mV
    // driveStraight(0, 0);
    // pros::delay(20);

    // forward 1/2 tile
    driveStraight(3000, 3000);
    pros::delay(1000);
    driveStraight(0, 0);
    pros::delay(20);

    // // forward 1/4 tile
    // driveStraight(6000, 6000);
    // pros::delay(300);
    // driveStraight(0, 0);
    // pros::delay(20);

    // arc motion right until have turned 90 degrees
    driveStraight(4500, 0);
    pros::delay(1500);  // modify this value experimentally
    driveStraight(0, 0);
    pros::delay(20);

    // expand wings
    wingsSolenoid.set_value(true);

    // arc motion right until have turned 90 degrees
    driveStraight(4500, 0);
    pros::delay(375);  // modify this value experimentally
    driveStraight(0, 0);
    pros::delay(20);

    // forward 1/4 tile into goal
    driveStraight(12000, 12000);
    pros::delay(500);
    driveStraight(0, 0);
    pros::delay(20);

    // back out of goal
    driveStraight(-12000, -12000);
    pros::delay(500);
    driveStraight(0, 0);
}

// Load Zone + Bar: LEFT+
void loadZoneAndBar() {
}

// Score Goal + Bar: RIGHT+
void scoreGoalAndBar() {}

/**
 * @brief Synchronous drive function that corrects for differences in friction
 * @param target The target voltage to drive at, [-12000, 12000]
 */
void driveStraight(int targetL, int targetR) {  // adjust for differences in friction
    int leftAdj = 1,
        rightAdj = 1.1;
    driveL.moveVoltage((int)(targetL * leftAdj));
    driveR.moveVoltage((int)(targetR * rightAdj));
}

void autonTest() {
    // begin auton dev
    // end auton dev
}

#endif
