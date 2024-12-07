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

// Global Variables
int PusherStartPosition;
int ArmStartPosition;

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

  std::cout << "IN PRE-AUTO\n";

  // Set motor brake modes
  armMotors.setStopping(brake);
  clamp.setStopping(hold);

  // Set pusher
  pusher.setVelocity(100, rpm);
  int PusherStartPosition = pusher.position(degrees) + 300;
  pusher.spinTo(PusherStartPosition, degrees);

  std::cout << "MOTORS SET\n";

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
  std::cout << "IN AUTO\n";

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
  std::cout << "\nIN TELEOP\n\n";

  int PusherStartPosition = pusher.position(degrees) + 300;
  int PusherEndPosition = 900;

  int ArmStartPosition = armMotors.position(degrees);
  int ArmEndPosition = -88;
  
  while (1) {

    // Basic tank drive
    leftFront.spin(forward, Controller.Axis3.position(), percent);  // Left Front Drive Code
    leftBack.spin(forward, Controller.Axis3.position(), percent);   // Left Back Drive Code
    rightFront.spin(forward, Controller.Axis2.position(), percent); // Right Front Drive Code
    rightBack.spin(forward, Controller.Axis2.position(), percent);  // Right Back Drive Code

    // Move arm -- Automated
    if (Controller.ButtonL1.pressing()) {
      std::cout << "arm up,   positon: " << armMotors.position(degrees) << "\n";
      armMotors.spinToPosition(ArmEndPosition, degrees);
    }
    else if (Controller.ButtonL2.pressing()) {
      std::cout << "arm down, position: " << armMotors.position(degrees) << "\n";
      armMotors.spinToPosition(ArmStartPosition, degrees);
    }

    // Move arm -- Manual
    if (Controller.ButtonR1.pressing()) {
      std::cout << "arm up, manual\n";
      armMotors.setVelocity(45, rpm);
      armMotors.spin(reverse);
    }
    else if (Controller.ButtonR2.pressing()) {
      std::cout << "arm down, manual\n";
      armMotors.setVelocity(45, rpm);
      armMotors.spin(forward);
    }
    else {
      armMotors.stop();
      armMotors.setVelocity(50,rpm);
    }

    // Move pusher -- Automated
    if (Controller.ButtonA.pressing()) {
      std::cout << "pusher,   position: " << pusher.position(degrees) << "\n";
      pusher.spinToPosition(PusherEndPosition, degrees);
      pusher.spinToPosition(PusherStartPosition, degrees);
    }
    else if (Controller.ButtonB.pressing()) {
      std::cout << "maual pusher down\n";
      pusher.spinToPosition(PusherEndPosition, degrees);
    }

    // Emergency Stop
    if (Controller.ButtonX.pressing()) {
      std::cout << "EMERGENCY STOP: RESET REQUIRED \n";
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


// BUG REPORT/TO-DOS -- 12/06/24, Leah Reed
//
// Every system on robot has basic functionality!
//
// General:
// * code basic auto
//
// Arm:
// * add manuel control on R1 & R2 - (play with velocity??)
//
// Pusher:
// * n/a
//
// Drive:
// * n/a
//
// Clamp:
// * not yet on robot