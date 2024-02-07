#ifndef AUTONOMOUS_CPP
#define AUTONOMOUS_CPP

#include "autonomous.h"

#define GOAL_X_POS 40

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
    intake.moveVelocity(-600);
    pros::delay(200);
    intake.moveVelocity(600);
    pros::delay(750);
    intake.moveVelocity(0);
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
    /* PID auton
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
    */

    // simple auton
    driveL.move_velocity(600);
    driveR.move_velocity(600);
    pros::delay(1000);
    driveL.move_velocity(0);
    driveR.move_velocity(0);
}

// Score Goal: RIGHT
void scoreGoal() {
    autonChassis.setPose(36,-60,0);
    // autonChassis.moveTo(36,-36,1000);
    // pros::delay(100);
    // autonChassis.turnTo(24,-24,1000);
    // pros::delay(100);
    // autonChassis.moveTo(24,-24,1000);
    // wingsSolenoid.set_value(true);
    // pros::delay(100);
    // autonChassis.turnTo(24,0,1000);
    // pros::delay(100);
    // autonChassis.moveTo(24,-6,1000);
    // pros::delay(100);
    // autonChassis.turnTo(60,-6,1000);
    // pros::delay(100);
    // autonChassis.moveTo(45,-6,1000);

    // try pros::delay(50);

    // alliance ball
    autonChassis.moveTo(36,-14,1000);
    autonChassis.turnTo(72,-14,1000);
    autonChassis.moveTo(GOAL_X_POS+5,-14,1000);
    intake.moveVelocity(-600);
    pros::delay(200);
    // (24,0) ball
    autonChassis.moveTo(36,-14,1000);
    autonChassis.turnTo(24,-2,1000);
    intake.moveVelocity(600);
    autonChassis.moveTo(24,-2,1000);
    autonChassis.turnTo(72,-2,1000);
    autonChassis.moveTo(GOAL_X_POS,-2,1000);
    intake.moveVelocity(-600);
    // (4,0) ball
    autonChassis.moveTo(24, -2, 1000);
    autonChassis.turnTo(0,-2,1000);
    intake.moveVelocity(600);
    autonChassis.moveTo(6,-2,1000);
    autonChassis.turnTo(72,-2,1000);
    autonChassis.moveTo(GOAL_X_POS,-2,1000);
    intake.moveVelocity(-600);
    // elevation bar
    autonChassis.moveTo(20,-2,1000);
    autonChassis.turnTo(0,-2,1000);
    intake.moveVelocity(0);
    // autonChassis.turnTo(36,-72,1000);
    // autonChassis.moveTo(36,-60,1000);
    // autonChassis.turnTo(0,-60,1000);
    // wingsSolenoid.set_value(true);
    // intake.moveVelocity(600);
    // autonChassis.moveTo(8,-60,1000);
}

// Load Zone + Bar: LEFT+
void loadZoneAndBar() {
}

// Score Goal + Bar: RIGHT+
void scoreGoalAndBar() {
    scoreGoal();
    wingsSolenoid.set_value(false);
    autonChassis.moveTo(36,-12,1000);
    pros::delay(100);
    autonChassis.turnTo(36,-60,1000);
    pros::delay(100);
    autonChassis.moveTo(36,-60,1000);
    pros::delay(100);
    autonChassis.turnTo(10,-60,1000);
    pros::delay(100);
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
    driveL.move_voltage((int)(targetL * leftAdj));
    driveR.move_voltage((int)(targetR * rightAdj));
}

void autonTest() {
    // begin auton dev
    // end auton dev
}

#endif
