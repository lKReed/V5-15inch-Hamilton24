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
controller Controller = controller();
motor leftFront = motor(PORT9, false);
motor leftBack = motor(PORT10, false);
motor rightFront = motor(PORT1, false);
motor rightBack = motor(PORT2, false);

motor pusher = motor(PORT3, false);
motor leftArm = motor(PORT4, false);
motor rightArm = motor(PORT5, false);


// define your global instances of motors and other devices here

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
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Basic tank drive
    leftFront.spin(forward, Controller.Axis3.position(), percent); // Left Front Drive Code
    leftBack.spin(forward, Controller.Axis3.position(), percent); // Left Back Drive Code
    rightFront.spin(forward, Controller.Axis2.position(), percent); // Right Front Drive Code
    rightBack.spin(forward, Controller.Axis2.position(), percent); // Right Back Drive Code

    // Move arm
    if (Controller.ButtonL1.pressing()) {
      leftArm.spin(forward);
      rightArm.spin(forward);
    }

    if (Controller.ButtonL2.pressing()) {
      leftArm.spin(reverse);
      rightArm.spin(reverse);
    }

    // Move pusher
    if (Controller.ButtonA.pressing()) {
      pusher.spin(forward);
    }

    if (Controller.ButtonB.pressing()) {
      pusher.spin(reverse);
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
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
