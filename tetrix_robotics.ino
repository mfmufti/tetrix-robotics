/*
 * Skills Ontario 2020 Robotics Competition Program
 * Group Members: Andy, Johana, Areeb, Brady, Ryan, Musab
 * Mini Golf Competition
 * 
 * This is the main file of the robotics competition program.
 * 
*/


// Include the "tools" library for functions and constants.

#include "tools.h"


// Setup the robot by calling the "setupRobot" function.

void setup()
{
	setupRobot();

	changeArmHeight(ARM_UP_DEGREES);

	// moveForward();
}


void hi()
{
	int wallState = checkWalls();

	if (wallState == STOP)
	{
		// Stop the program.
		move(100, -150);
		delay(100);
		prizm.PrizmEnd();
	}

	moveToNextCourse(wallState);
}

// Declare variables.

bool leftFirst = false;
bool foundLine;
bool ballPicked; // Not used yet.
bool wide = true;

int iterations = 0;

void mainProgram()
{
	// for (int i = 0; i < 100; i++)
	// {
	// 	isLine();
	// 	delay(1);
	// }

	if (isLine())
	{
		
		// if (iterations > 100)
		// {
		// 	wide = false;
		// }

		moveForward();
		while (isLine()) {
			delay(1);
			// ++iterations;
			// if (itera)
		}
		stopMotors();
	}
	else
	{
		wide = true;
		iterations = 0;
	}
	

	delay(500);

	if (leftFirst)
	{
		foundLine = !turnToLine(LEFT, true) && !turnToLine(RIGHT, true);
	}
	else
	{
		foundLine = !turnToLine(RIGHT, true) && !turnToLine(LEFT, true);
	}

	leftFirst = !leftFirst;
	
	if (foundLine)
	{
		switch (checkLineState())
		{
			case noLine:
				print("noLine");

				if (!ballPicked)
				{
					// pick up the ball

					// prerequisites: open the claw, adjust the arm, and maybe back up
					move(OBJECT_MOVEMENT_SPEED, OBJECT_BACKWARD_DEGREES * -1);
					changeArmHeight(ARM_DOWN_DEGREES);
					openClaw();
					// step 1: turn to locate the ball (skip for now)
					// step 2: correct distance by backing up or moving forward
					adjustObjectDistance(CLAW_SONIC_SENSOR_PORT, BALL_IDEAL_DISTANCE, OBJECT_MAX_DISTANCE);
					// step 3: determine whether ball is at high or low height
					if (objectHighOrLow() == HIGH)
					{
					// step 4: move arm to correct ball height
						changeArmHeight(ARM_HIGH_BALL_DEGREES);
						move(OBJECT_MOVEMENT_SPEED, HIGH_BALL_FORWARD_DEGREES);
					}
					else
					{
						changeArmHeight(ARM_LOW_BALL_DEGREES);
						move(OBJECT_MOVEMENT_SPEED, LOW_BALL_FORWARD_DEGREES);
					}
					// step 5: move specific degrees forward  IT IS UP
					// step 6: close the claw
					closeClaw();
					// step 7: move arm up

					move(100, -100);
					changeArmHeight(ARM_UP_DEGREES);
					// step 8: move to original position

					ballPicked = true;
				}

				uTurn();
				break;
			case tee:
				print("tee");

				if (ballPicked)
				{
					// drop the ball

					// prerequisites: adjust the arm, and maybe back up
					// changeArmHeight(ARM_DOWN_DEGREES);
					// move(OBJECT_MOVEMENT_SPEED, OBJECT_BACKWARD_DEGREES * -1);
					// // step 1: turn to locate the cup (skip for now)
					// // step 2: correct distance by backing up or moving forward
					adjustObjectDistance(STATIC_SONIC_SENSOR_PORT, CUP_IDEAL_DISTANCE, OBJECT_MAX_DISTANCE);
					// step 3: move specific degrees backward
					move(OBJECT_MOVEMENT_SPEED, CUP_BACKWARD_DEGREES * -1);
					// step 4: move arm to correct cup height
					changeArmHeight(ARM_CUP_DEGREES);
					// step 5: open the claw
					openClaw();
					// step 6: move arm up
					changeArmHeight(ARM_UP_DEGREES);
					// step 7: move to original position

					// turn to the next "course"
					
					delay(1000);

					

					hi();
					// wide = true;

					changeArmHeight(ARM_UP_DEGREES);

					ballPicked = false;
				}
				else
				{
					// make a u turn

					uTurn();
				}

				// while(1) {}
				break;
			case rightTurn:
				bigTurn(RIGHT);
				break;
			case leftTurn:
				bigTurn(LEFT);
				break;
		}
	}
}


// Loop infinitely to follow the line, and turn when needed.  When a line break is reached, perform
// the appropriate action such as pick up a ball or drop a ball.

unsigned int distance, lastDistance = 1000000;

void loop()
{
	// Test the main program.
	mainProgram();

	// Test the "course changing" program.
	// hi();
	// delay(10000);

	// adjustObjectDistance(STATIC_SONIC_SENSOR_PORT, CUP_IDEAL_DISTANCE, OBJECT_MAX_DISTANCE);
	// // step 3: move specific degrees backward
	// move(OBJECT_MOVEMENT_SPEED, CUP_BACKWARD_DEGREES * -1);
	// // step 4: move arm to correct cup height
	// changeArmHeight(ARM_CUP_DEGREES);
	// // step 5: open the claw
	// openClaw();
	// // step 6: move arm up
	// changeArmHeight(ARM_UP_DEGREES);
	// // step 7: move to original position

	// // turn to the next "course"
	
	// int wallState = checkWalls();

	// if (wallState == STOP)
	// {
	// 	// Stop the program.

	// 	while (1) {}
	// }

	// moveToNextCourse(wallState);

	// ballPicked = false;

	// delay(10000);

	// Test the "course changing" program.

	// if (!checkWalls())
	// {
	// 	// Stop the program.

	// 	while (1) {}
	// }

	// moveToNextCourse(LEFT);

	// delay (10000);

	// bigTurn(RIGHT);


	// Test the static ultrasonic sensor.

	// distance = getObjectDistance(STATIC_SONIC_SENSOR_PORT, WALL_MAX_DISTANCE);
	// // distance = prizm.readSonicSensorCM(STATIC_SONIC_SENSOR_PORT);

	// if (distance != lastDistance)
	// {
	// 	print("Distance: ");
	// 	print(distance);
	// 	print("");
	// 	lastDistance = distance;
	// }

	// lastDistance = distance;

	// delay(100);


	// Test the ball dropping program.

	// adjustObjectDistance(STATIC_SONIC_SENSOR_PORT, CUP_IDEAL_DISTANCE, OBJECT_MAX_DISTANCE);
	// // step 3: move arm to correct cup height
	// changeArmHeight(ARM_CUP_DEGREES);
	// // step 4: move specific degrees backward
	// move(OBJECT_MOVEMENT_SPEED, CUP_BACKWARD_DEGREES * -1);
	// // step 5: open the claw
	// openClaw();
	// // step 6: move arm up
	// changeArmHeight(ARM_UP_DEGREES);
	// // step 7: move to original position


	// Ball dropper test.

	// adjustObjectDistance(STATIC_SONIC_SENSOR_PORT, CUP_IDEAL_DISTANCE, OBJECT_MAX_DISTANCE);
	// // step 3: move arm to correct cup height
	// changeArmHeight(ARM_CUP_DEGREES);
	// // step 4: move specific degrees backward
	// move(OBJECT_MOVEMENT_SPEED, CUP_BACKWARD_DEGREES * -1);
	// // step 5: open the claw
	// openClaw();

	// delay(10000);
	// // step 6: move arm up
	// changeArmHeight(ARM_UP_DEGREES);

	// changeArmHeight(ARM_LOW_BALL_DEGREES);
	// delay(5000);
	// changeArmHeight(ARM_HIGH_BALL_DEGREES);
	// delay(5000);


	// Wednesday, January 22, 2020 - List of Tests to Perform

	// 1. Run the main program, except that the program stops when a left turn is made to check for
	// the new course.  Instead of continuing, report the readings, with delays.

	// 2. Check to see the reading on the left turn when only the course changing program is tested
	// (not part of the main program).

	// 3. Modify the ultrasonic reader function (getObjectDistance) so that it uses multiple
	// readings, with possible delays in between the readings.

	// 4. Maybe run the course changer alone, so that the robot starts closer to the wall?
}