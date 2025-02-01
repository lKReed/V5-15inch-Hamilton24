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

timer Timer;

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

// Axis 2 forward/backwards
// Axis 4 left/right
void alexDrive() {
  // velocity formula for exponential speed instead of linear speed
  double velocity2 = (pow(abs(Controller.Axis2.position()), 1.43) / 1000) * 100;
  if (Controller.Axis2.position() < 0)
    velocity2 *= -1;

  double leftVelocity = velocity2;
  double rightVelocity = velocity2;

  double velocity4 = (pow(abs(Controller.Axis4.position()), 1.43) / 1000) * 100;
  if (Controller.Axis4.position() < 0)
    velocity4 *= -1;

  // subtract (and add) the value of left/right velocity from the opposite wheel to turn
  if (velocity4 > 0) {
    rightVelocity -= abs(velocity4);
    leftVelocity += abs(velocity4);
  }
  else if (velocity4 < 0) {
    leftVelocity -= abs(velocity4);
    rightVelocity += abs(velocity4);
  }

  leftDrive.spin(forward, leftVelocity, percent);
  rightDrive.spin(forward, rightVelocity, percent);
}

void usercontrol(void) {
  // User control code here, inside the loop
  std::cout << "\nIN TELEOP\n\n";

  int PusherUpPosition = pusher.position(degrees);
  int PusherStartPosition = pusher.position(degrees) + 300;
  int PusherEndPosition = 900;

  int ArmStartPosition = armMotors.position(degrees);
  int ArmEndPosition = -150;
  
  while (1) {
    Timer.reset();
    while (Timer.time(sec) < 3) {

      // Drive Code
      alexDrive();

      // Move arm -- Automated
      if (Controller.ButtonL1.pressing()) {
        std::cout << "arm up,   positon: " << armMotors.position(degrees) << "\n";
        armMotors.setStopping(hold);
        armMotors.spinToPosition(ArmEndPosition, degrees);
      }
      else if (Controller.ButtonL2.pressing()) {
        armMotors.setStopping(brake);
        armMotors.setVelocity(90, rpm);
        std::cout << "arm down, position: " << armMotors.position(degrees) << "\n";
        armMotors.spinToPosition(ArmStartPosition, degrees);
        armMotors.setVelocity(50, rpm);
      }

      // Move arm -- Manual
      if (Controller.ButtonUp.pressing()) {
        std::cout << "arm up,   manual\n";
        armMotors.setVelocity(45, rpm);
        armMotors.spin(reverse);
      }
      else if (Controller.ButtonDown.pressing()) {
        std::cout << "arm down, manual\n";
        armMotors.setVelocity(45, rpm);
        armMotors.spin(forward);
      }
      else {
        armMotors.stop();
        armMotors.setVelocity(50,rpm);
      }

      // Move pusher -- Automated
      if (Controller.ButtonR1.pressing()) {
        std::cout << "pusher,   position: " << pusher.position(degrees) << "\n";
        pusher.spinToPosition(PusherEndPosition, degrees);
        pusher.spinToPosition(PusherStartPosition, degrees);
      }
      else if (Controller.ButtonR2.pressing()) {
        std::cout << "maual pusher down\n";
        pusher.spinToPosition(PusherEndPosition, degrees);
      }

      // Reset
      if (Controller.ButtonY.pressing()) {
        std::cout << "reset pusher to top \n";
        pusher.spinToPosition(PusherUpPosition, degrees);
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