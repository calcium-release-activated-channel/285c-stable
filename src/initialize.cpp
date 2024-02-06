#ifndef INITIALIZE_CPP
#define INITIALIZE_CPP

#include "main.h"

/* Key:
 * "*" == Working
 * "-" == Programmed but untested
 * " " == Not programme
 */
const std::vector<std::string> autonModes = {
    "* (R) ScrGoal",
    "* No Auton   ",
    "* (L) LoadZn ",
    "  (+) Elev Br",
    "  (L+) LdZ+Br",
    "  (R+) ScG+Br",
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
    // if (cataSubhandlerTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING) {
    //     cataSubhandlerTask.suspend();
    // }
    // if (intakeSubhandlerTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING) {
    //     intakeSubhandlerTask.suspend();
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
Motor driveLF(driveLFPort, true, motorSetting);
Motor driveLU(driveLUPort, false, motorSetting);
Motor driveLB(driveLBPort, true, motorSetting);
Motor driveRF(driveRFPort, false, motorSetting);
Motor driveRU(driveRUPort, true, motorSetting);
Motor driveRB(driveRBPort, false, motorSetting);

Motor fw(fwPort, false, motorSetting);
Motor intake(intakePort, true, motorSetting);

// sensors
pros::ADIDigitalIn autonSelector(autonSelectorPort);
pros::Imu imuObj(imuPort);

// solenoid
pros::ADIDigitalOut elevSolenoid(elevPort);
pros::ADIDigitalOut wingsSolenoid(wingsPort);

// motor groups
MotorGroup driveL({driveLF, driveLU, driveLB});
MotorGroup driveR({driveRF, driveRU, driveRB});

// shadow copy for using pros instead of okapi lemlib (don't ask)
pros::Motor driveLAutonF(-driveLFPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveLAutonU(driveLUPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveLAutonB(-driveLBPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveRAutonF(driveRFPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveRAutonU(-driveRUPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor driveRAutonB(driveRBPort, pros::E_MOTOR_GEAR_BLUE);
pros::Motor_Group driveLAuton({driveLAutonF, driveLAutonU, driveLAutonB});
pros::Motor_Group driveRAuton({driveRAutonF, driveRAutonU, driveRAutonB});

// PID
lemlib::Drivetrain_t autonDrive{
    &driveLAuton,  // left drivetrain motors
    &driveRAuton,  // right drivetrain motors
    9.5,           // track width
    4,             // wheel diameter
    300            // wheel rpm
};
lemlib::ChassisController_t linearController{10,   // kP
                                             30,   // kD
                                             1,    // inner error bound (in)
                                             100,  // inner error timeout (ms)
                                             3,    // outer error bound (in)
                                             500,  // outer error timeout (ms)
                                             20    // max acceleration (slew)
};
lemlib::ChassisController_t angularController{2,   // kP
                                              10,   // kD
                                              1,    // inner error bound (in)
                                              100,  // inner error timeout (ms)
                                              3,    // outer error bound (in)
                                              500,  // outer error timeout (ms)
                                              0    // max acceleration (slew)
};
lemlib::OdomSensors_t sensors{
    nullptr, nullptr, nullptr, nullptr, &imuObj  // imu
};

lemlib::Chassis autonChassis(autonDrive, linearController, angularController, sensors);

// drive
std::shared_ptr<ChassisController> drive = okapi::ChassisControllerBuilder()
                                               .withMotors(driveL, driveR)
                                               .withDimensions({AbstractMotor::gearset::blue, (36.0 / 72.0)}, {{4_in, 9.5_in}, imev5BlueTPR})  // 4 inch wheels, 12 inch track width, where track width refers to the distance between the left and right wheels measured from the centers of the wheels
                                               .withMaxVelocity(600)
                                               .build();
std::shared_ptr<okapi::ChassisModel> model =
    std::dynamic_pointer_cast<okapi::ChassisModel>(drive->getModel());

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    taskKill();  // kill initialized programs
    driveL.setBrakeMode(AbstractMotor::brakeMode::brake);
    driveR.setBrakeMode(AbstractMotor::brakeMode::brake);
    intake.setBrakeMode(AbstractMotor::brakeMode::brake);
    fw.setBrakeMode(AbstractMotor::brakeMode::brake);
    driveLAuton.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    driveRAuton.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    // idk if i have to retract pneumatics but just in case ig
    elevSolenoid.set_value(false);
    wingsSolenoid.set_value(false);
    imuObj.reset();
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
