#ifndef INITIALIZE_CPP
#define INITIALIZE_CPP

#include "main.h"

/* Key:
 * "*" == Working
 * "-" == Programmed but untested
 * " " == Not programmed
 */
const std::vector<std::string> autonModes = {
    "* (R) ScrGoal",
    "* No Auton   ",
    "- (L) LoadZn ",
    "  (L+) LdZ+Br",
    "  (R') LdZElm",
    "  Auton Test "};
int autMode = 0;

/*** TASK SAFETY ***/
// safety
void taskKill() {
    if (buttonInterruptsTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING) {
        buttonInterruptsTask.suspend();
    }
    // if (armAutonTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING) {
    //     armAutonTask.suspend();
    // }
}

/*** BEGIN PORTS AND CONTROLLER DECLARATIONS ***/
// controls
ControllerButton fwBtn = ControllerDigital::up;
ControllerButton fwRevBtn = ControllerDigital::down;
ControllerButton wingsBtn = ControllerDigital::L1;
ControllerButton intakeBtn = ControllerDigital::R1;
ControllerButton outtakeBtn = ControllerDigital::R2;
ControllerButton endgameBtn = ControllerDigital::B;

// ports
// drive motors
int8_t driveLFPort = 11;
int8_t driveLUPort = 12;
int8_t driveLBPort = 13;
int8_t driveRFPort = 20;
int8_t driveRUPort = 19;
int8_t driveRBPort = 18;

// auxiliary motors
int8_t intakePort = 17;
int8_t fwPort = 14;

// sensors (implicit conversion)
uint8_t autonSelectorPort = 'H';
uint8_t wingsPort = 'G';
uint8_t elevPort = 'F';
uint8_t imuPort = 15;
/*** END PORTS AND CONTROLLER DECLARATIONS ***/

// controller
Controller controller;

// motor
pros::Motor driveLF(-driveLFPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveLU(driveLUPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveLB(-driveLBPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveRF(driveRFPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveRU(-driveRUPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveRB(driveRBPort, pros::E_MOTOR_GEAR_BLUE);

Motor fw(fwPort, false, motorSetting);
Motor intake(intakePort, true, motorSetting);

// sensors
pros::ADIDigitalIn autonSelector(autonSelectorPort);
pros::Imu imuObj(imuPort);

// solenoid
pros::ADIDigitalOut elevSolenoid(elevPort);
pros::ADIDigitalOut wingsSolenoid(wingsPort);

// motor groups
pros::Motor_Group driveL({driveLF, driveLU, driveLB});
pros::Motor_Group driveR({driveRF, driveRU, driveRB});

// PID
lemlib::Drivetrain_t autonDrive{&driveL, &driveR, 9.5, 4, 300};
lemlib::ChassisController_t linearController{10, 30, 1, 100, 3, 500, 20};
lemlib::ChassisController_t angularController{2, 10, 1, 100, 3, 500, 0};
lemlib::OdomSensors_t sensors{nullptr, nullptr, nullptr, nullptr, &imuObj};
lemlib::Chassis autonChassis(autonDrive, linearController, angularController, sensors);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    taskKill();  // kill initialized programs
    intake.setBrakeMode(AbstractMotor::brakeMode::brake);
    fw.setBrakeMode(AbstractMotor::brakeMode::coast);
    driveL.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    driveR.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    // idk if i have to retract pneumatics but just in case ig
    elevSolenoid.set_value(false);
    wingsSolenoid.set_value(false);
    // imuObj.reset();
    autonChassis.calibrate();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    taskKill();  // auton -> disable -> opcontrol
    controller.setText(0, 0, "Disabled :/  ");
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
    taskKill();  // if comp cable is unplugged
    pros::delay(100);
    controller.setText(0, 0, autonModes[autMode]);
    while (true) {
        if (autonSelector.get_new_press()) {
            controller.rumble(".");
            autMode = autMode < (int)autonModes.size() - 1 ? autMode + 1 : 0;
            controller.clearLine(0);
            pros::delay(50);
            controller.setText(0, 0, autonModes[autMode]);
        }
        pros::delay(20);
    }
}

#endif
