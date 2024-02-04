#ifndef PID_H
#define PID_H

#include "main.h"

/**
 * @brief Class for a PID controller.
 */
class PID {
   private:  // only put persistent variables here
    // used in whole class -----------------------
    double kP;       /** Proportional constant */
    double kI;       /** Integral constant */
    double kD;       /** Derivative constant */
    double Ibound;   /** Range of acceptable integral values */
    double outBound; /** Range of acceptable output values */
    // used per movement -------------------------
    double target; /** The desired value */
    // used per loop -----------------------------
    double integral;   /** Integral value */
    double prevError;  /** Previous distance to target*/

   public:
    /**
     * @brief Constructor for PID class. Tune your PID constants here.
     * @param kP Proportional gain
     * @param kI Integral gain
     * @param kD Derivative gain
     * @param Ibound Range of acceptable integral values
     * @param outBound [Optional] Range of acceptable output values (Voltage: 12000, Ignore: 0)
     */
    PID(double kP, double kI, double kD, double Ibound, double outBound = 0);
    /**
     * @brief Function to set the target of the PID controller.
     * @param new_target The new target
     */
    void setTarget(double new_target);
    /**
     * @brief Function to calculate the output of the PID controller.
     * @param new_current The current value
     * @return The output of the PID controller
     */
    double calculatePID(double new_current);
    /**
     * @brief A destructor for the PID class to prevent memory leaks.
     */
    ~PID();
};

#endif