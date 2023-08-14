/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convienent for most student programmers.
 */
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"

/**
 * You should add more #includes here
 */
#include "okapi/api.hpp"
#include "pros/apix.h"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
using namespace okapi;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif

#define driveSetting AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees

extern int autMode;

extern Controller controller;

extern ControllerButton cataBtn;
extern ControllerButton intakeBtn;
extern ControllerButton outtakeBtn;
extern ControllerButton endgameBtn;
extern ControllerButton wingsBtn;

extern Motor driveLF;
extern Motor driveLB;
extern Motor driveRF;
extern Motor driveRB;
extern Motor shiftL;
extern Motor shiftR;

extern Motor cata;
extern Motor intake;

extern pros::adi::DigitalIn cataStop;
extern pros::adi::DigitalIn autonSelector;
extern pros::adi::DigitalOut shiftSolenoid;
extern pros::adi::DigitalOut wingsSolenoid;
// extern pros::adi::DigitalOut sweepSolenoid;

extern MotorGroup driveL;
extern MotorGroup driveR;
extern MotorGroup endgame;

extern std::shared_ptr<ChassisController> drive;

void autonomous();
void initialize();
void disabled();
void competition_initialize();
void opcontrol();

/**
 * @brief Class for a PID controller.
 */
class PID {
   private:
    double kP;             /** Proportional constant */
    double kI;             /** Integral constant */
    double kD;             /** Derivative constant */
    double IconstrainLow;  /** Lowest acceptable integral value */
    double IconstrainHigh; /** Highest acceptable integral value */
    double prevError;      /** Previous distance to target*/
    uint32_t prevTime;     /** Previous time measured*/

   public:
    /**
     * @brief Constructor for PID class. Tune your PID constants here.
     * @param kP Proportional gain
     * @param kI Integral gain
     * @param kD Derivative gain
     */
    PID(double kP, double kI, double kD, double IconstrainLow, double IconstrainHigh);
    /**
     * @brief Function to call to calculate the output of the PID controller.
     * @param new_target The desired value
     * @param new_current The current value
     * @return The output of the PID controller
     */
    double calculatePID(double new_target, double new_current);
    /**
     * @brief Destroy the PID object
     *
     */
    ~PID();
};

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
// #include <iostream>
#endif

#endif  // _PROS_MAIN_H_
