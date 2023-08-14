#ifndef INITIALIZE_CPP
#define INITIALIZE_CPP

#include "main.h"

const std::vector<std::string> autonModes = {
    "No Autonomous",
    "Elevation Bar",
    "Load Zone",
    "Score Goal",
    "Load Zone + Bar",
    "Score Goal + Bar"};
int autMode = 0;

/*** BEGIN PORTS AND CONTROLLER DECLARATIONS ***/
// controls
ControllerButton cataBtn = ControllerDigital::L1;
ControllerButton wingsBtn = ControllerDigital::L2;
ControllerButton intakeBtn = ControllerDigital::R1;
ControllerButton outtakeBtn = ControllerDigital::R2;
ControllerButton endgameBtn = ControllerDigital::A;

// ports
// drive motors
int8_t driveLFPort = 1;
int8_t driveLBPort = 2;
int8_t driveRFPort = 11;
int8_t driveRBPort = 12;

// transmission motors
int8_t shiftLPort = 3;
int8_t shiftRPort = 13;

// intake motor
int8_t intakePort = 4;

// cata motor
int8_t cataPort = 5;

// sensors (implicit conversion)
uint8_t cataStopPort = 'A'; // might remove with cata redesign
uint8_t autonSelectorPort = 'B';
uint8_t shiftSolenoidPort = 'C';
uint8_t wingsSolenoidPort = 'D';
// uint8_t sweepSolenoidPort = 'E';
/*** END PORTS AND CONTROLLER DECLARATIONS ***/

// controller
Controller controller;

// motor
Motor driveLF(driveLFPort, true, driveSetting);
Motor driveLB(driveLBPort, true, driveSetting);
Motor driveRF(driveRFPort, false, driveSetting);
Motor driveRB(driveRBPort, false, driveSetting);
Motor shiftL(shiftLPort, false, driveSetting);
Motor shiftR(shiftRPort, true, driveSetting);
Motor cata(cataPort, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor intake(intakePort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

// sensors
pros::adi::DigitalIn cataStop(cataStopPort);
pros::adi::DigitalIn autonSelector(autonSelectorPort);

// solenoid
pros::adi::DigitalOut shiftSolenoid(shiftSolenoidPort);
pros::adi::DigitalOut wingsSolenoid(wingsSolenoidPort);
// pros::adi::DigitalOut sweepSolenoid(sweepSolenoidPort);

// motor groups
MotorGroup driveL({driveLF, driveLB, shiftL});
MotorGroup driveR({driveRF, driveRB, shiftR});
MotorGroup endgame({shiftL, shiftR});
// in case we go back to 4 motor drive
// MotorGroup driveL({driveLF, driveLB});
// MotorGroup driveR({driveRF, driveRB});

// drive
std::shared_ptr<ChassisController> drive = okapi::ChassisControllerBuilder()
                                               .withMotors(driveL, driveR)
                                               .withDimensions({AbstractMotor::gearset::green, (72.0 / 48.0)}, {{4_in, 12_in}, imev5GreenTPR})  // 4 inch wheels, 12 inch track width, where track width refers to the distance between the left and right wheels measured from the centers of the wheels
                                               .withMaxVelocity(200)
                                               .build();

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    driveL.setBrakeMode(AbstractMotor::brakeMode::coast);
    driveR.setBrakeMode(AbstractMotor::brakeMode::coast);
    // idk if i have to retract pneumatics but just in case ig
    shiftSolenoid.set_value(false);
    wingsSolenoid.set_value(false);
    // sweepSolenoid.set_value(false);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    controller.setText(0,0,"Disabled :/");
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
    controller.setText(0, 0, "Auton Mode:");
    controller.setText(1, 0, autonModes[autMode]);
    while (true) {
        if (autonSelector.get_new_press()) {
            controller.rumble(".");
            autMode = autMode < (int)autonModes.size() ? autMode + 1 : 0;
            controller.clearLine(1);
            controller.setText(1, 0, autonModes[autMode]);
        }
        pros::delay(20);
    }
}

#endif