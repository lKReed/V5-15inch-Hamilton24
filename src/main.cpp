/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       lkree                                                     */
/*    Created:      11/9/2024, 11:53:09 AM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "math.h"
#include <iostream>
using namespace vex;

// A global instance of competition
competition Competition;

// Motor and Controller definitions
controller Controller = controller();

motor leftFront = motor(PORT9, true);
motor leftMiddle = motor(PORT8, false);
motor leftBack = motor(PORT7, true);
motor_group leftDrive = motor_group(leftFront, leftMiddle, leftBack);

motor rightFront = motor(PORT10, true);
motor rightMiddle = motor(PORT11, false);
motor rightBack = motor(PORT12, true);
motor_group rightDrive = motor_group(rightFront, rightMiddle, rightBack);

motor grabber = motor(PORT1, true);

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

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// Axis 2 forward/backwards
// Axis 4 left/right
void alexDrive() {
  // velocity formula for exponential speed instead of linear speed
  double forwardsVelocity = (pow(abs(Controller.Axis2.position()), 1.43) / 1000) * 100;
  if (Controller.Axis2.position() < 0)
    forwardsVelocity *= -1;

  double leftVelocity = forwardsVelocity;
  double rightVelocity = forwardsVelocity;

  double sidewaysVelocity = (pow(abs(Controller.Axis4.position()), 1.43) / 1000) * 100;
  if (Controller.Axis4.position() < 0)
    sidewaysVelocity *= -1;

  // subtract/add the value of left/right velocity from the opposite wheel to turn
  if (sidewaysVelocity > 0) {
    rightVelocity -= abs(sidewaysVelocity);
    leftVelocity += abs(sidewaysVelocity);
  }
  else if (sidewaysVelocity < 0) {
    leftVelocity -= abs(sidewaysVelocity);
    rightVelocity += abs(sidewaysVelocity);
  }

  // drive at determined velocity
  leftDrive.spin(forward, leftVelocity, percent);
  rightDrive.spin(forward, rightVelocity, percent);
}

void usercontrol(void) {
  // User control code here, inside the loop
  std::cout << "\nIN TELEOP\n\n";

  grabber.setVelocity(70, rpm);
  
  while (1) {

    // Drive Code
    alexDrive();

    // Move goal grabber
    if (Controller.ButtonL1.pressing()) {
      std::cout << "grabber,   position: " << grabber.position(degrees) << "\n";
      grabber.spin(forward);
    }
    else if (Controller.ButtonL2.pressing()) {
      std::cout << "grabber,   position: " << grabber.position(degrees) << "\n";
      grabber.spin(reverse);
    }
    else {
      grabber.stop();
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