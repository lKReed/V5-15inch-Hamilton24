/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       lkree                                                     */
/*    Created:      11/9/2024, 11:53:09 AM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// Motor and Controller definitions
controller Controller = controller();

motor leftFront = motor(PORT9, false);
motor leftBack = motor(PORT10, false);
motor_group leftDrive = motor_group(leftFront, leftBack);

motor rightFront = motor(PORT1, true);
motor rightBack = motor(PORT2, true);
motor_group rightDrive = motor_group(rightFront, rightBack);

motor pusher = motor(PORT3, false);

motor clamp = motor(PORT7, false);

motor leftArm = motor(PORT4, false);
motor rightArm = motor(PORT5, true);
motor_group armMotors = motor_group(leftArm, rightArm);

// Global Variables (Constants)
int PusherStartPosition = 0;
int ArmStartPosition = 0;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  // Set motor brake modes
  armMotors.setStopping(hold);
  clamp.setStopping(hold);

  leftFront.setStopping(brake);
  leftBack.setStopping(brake);
  rightFront.setStopping(brake);
  rightBack.setStopping(brake);

  // Set motors to correct position for auto
  pusher.setPosition(PusherStartPosition, degrees);
  armMotors.setPosition(ArmStartPosition, degrees);

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  leftDrive.spinFor(forward, 30, degrees);
  rightDrive.spinFor(forward, 30, degrees);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // Basic tank drive
    leftFront.spin(forward, Controller.Axis3.position(), percent);  // Left Front Drive Code
    leftBack.spin(forward, Controller.Axis3.position(), percent);   // Left Back Drive Code
    rightFront.spin(forward, Controller.Axis2.position(), percent); // Right Front Drive Code
    rightBack.spin(forward, Controller.Axis2.position(), percent);  // Right Back Drive Code

    // Move arm
    if (Controller.ButtonL2.pressing()) {
      if (armMotors.position(degrees) < 50)
        armMotors.spin(forward);
    }
    else if (Controller.ButtonL1.pressing()) {
      armMotors.spin(reverse);
    }

    // Move pusher
    if (Controller.ButtonA.pressing()) {
      pusher.spin(forward);
      if (pusher.isDone()) {
        pusher.setPosition(PusherStartPosition, degrees);
      }
    }
    else if (Controller.ButtonB.pressing()) {
      pusher.spin(reverse);
    }

    if (Controller.ButtonX.pressing()) {
      return;
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}


// BUG REPORT -- 12/03/24, Leah Reed
//
// Pusher:
// * need to figure out why it doesn't reset - look more into how .done() works?
//
// Arm:
// * need to add something so the arm goes up immediatly so we can drive
// * need to figure out how to restrict movement of the arm
// * solution to overheating???