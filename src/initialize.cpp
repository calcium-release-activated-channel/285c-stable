#ifndef INITIALIZE_CPP
#define INITIALIZE_CPP

#include "main.h"

/*** TASK SAFETY ***/
// safety
void taskKill() {
    if (buttonInterruptsTask.get_state() == pros::task_state_e_t::E_TASK_STATE_RUNNING)
        buttonInterruptsTask.suspend();
}

/*** BEGIN PORTS AND CONTROLLER DECLARATIONS ***/
// controls
ControllerButton intakeBtn = ControllerDigital::R1;
ControllerButton outtakeBtn = ControllerDigital::R2;
ControllerButton clampBtn = ControllerDigital::L1;

// ports
// drive motors
int8_t driveLFPort = 20;
int8_t driveLUPort = 9;
int8_t driveLBPort = 19;
int8_t driveRFPort = 11;
int8_t driveRUPort = 13;
int8_t driveRBPort = 12;

// auxiliary motors
int8_t intakePort = 10;
int8_t conveyorPort = 5;

// sensors (implicit conversion)
uint8_t clampPort = 'F';
/*** END PORTS AND CONTROLLER DECLARATIONS ***/

// controller
Controller controller;

// motor
Motor driveLF(driveLFPort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor driveLU(driveLUPort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor driveLB(driveLBPort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor driveRF(driveRFPort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor driveRU(driveRUPort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor driveRB(driveRBPort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

Motor conveyor(conveyorPort, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor intake(intakePort, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

// solenoid
pros::ADIDigitalOut clampSolenoid(clampPort);

// motor groups
MotorGroup driveL({driveLF, driveLU, driveLB});
MotorGroup driveR({driveRF, driveRU, driveRB});

bool clampState = false; // this is the default state (open)

void initialize() {
    taskKill();  // kill initialized programs
    intake.setBrakeMode(AbstractMotor::brakeMode::brake);
    conveyor.setBrakeMode(AbstractMotor::brakeMode::brake);
    driveL.setBrakeMode(AbstractMotor::brakeMode::brake);
    driveR.setBrakeMode(AbstractMotor::brakeMode::brake);
    clampSolenoid.set_value(clampState);
}

void disabled() {
    taskKill();
    controller.setText(0, 0, "Disabled :/  ");
}

void competition_initialize() {
    taskKill();
}

#endif
