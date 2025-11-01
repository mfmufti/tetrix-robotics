/*
 * Skills Ontario 2020 Robotics Competition Program
 * Group Members: Andy, Johana, Areeb, Brady, Ryan, Musab
 * Mini Golf Competition
 * 
 * This file contains necessary "tools" such as functions and constants to
 * be used in the main program.
 * 
*/


// Prevent file from being imported multiple times.

#ifndef TOOLS
#define TOOLS


// Include necessary libraries.

#include <PRIZM.h>
#include <Arduino.h> 


// Define constants.

#define LEFT 1
#define RIGHT 0
#define STOP 2
#define HIGH true
#define LOW false

#define STRAIGHT_RATIO 1

#define BIG_TURN_SPEED 400
#define BIG_TURN_DEGREES 402
#define BIG_TURN_RATIO -2.5

// The following values may need to be adjusted and perhaps removed.
#define U_TURN_SPEED 400
#define U_TURN_DEGREES 43
#define U_TURN_RATIO -1.35
#define U_TURN_BACKWARD_DEGREES 600

#define LINE_TURN_EXTRA_RATIO -2
#define LINE_TURN_SPEED 50
#define LINE_TURN_DEGREES 75
#define LINE_TURN_RATIO 100
#define LINE_TURN_EXTRA_SPEED 30
#define LINE_TURN_EXTRA_DEGREES 15

#define TEE_TURN_SPEED 100
#define TEE_TURN_DEGREES 95 // Was 100. Changed to make turn narrower.
#define TEE_TURN_RATIO -4
#define TEE_TURN_BACKWARD_DEGREES 80
#define TEE_TURN_FORWARD_CHECK_DEGREES 55

#define FORWARD_SPEED 200

#define CLAW_SPEED 100
#define CLAW_TIME 3000
#define CLAW_PORT 1
#define CLAW_OPEN_DEGREES 83
#define CLAW_CLOSE_DEGREES 2

#define ARM_PORT 2
#define ARM_TIME 5000
#define ARM_SPEED 100
#define ARM_UP_DEGREES 180
#define ARM_DOWN_DEGREES 0
#define ARM_LOW_BALL_DEGREES 7
#define ARM_HIGH_BALL_DEGREES 57
#define ARM_CUP_DEGREES 100
#define ARM_BALL_CHECK_DEGREES 65 // Needs to be adjusted

#define LINE_SENSOR_PORT 4
#define LINE_CHECK_ITERATIONS 100

#define CLAW_SONIC_SENSOR_PORT 3
#define STATIC_SONIC_SENSOR_PORT 2
#define SONIC_READING_COUNT 100

// These all need to be adjusted.
#define OBJECT_MAX_DISTANCE 30
#define WALL_MAX_DISTANCE 230
#define BALL_IDEAL_DISTANCE 10
#define CUP_IDEAL_DISTANCE 11

// These all need to be adjusted.
#define LOW_BALL_FORWARD_DEGREES 100
#define HIGH_BALL_FORWARD_DEGREES 57
#define CUP_BACKWARD_DEGREES 130

#define OBJECT_BACKWARD_DEGREES 200  // This may need to be separated into two variables.
#define OBJECT_MOVEMENT_SPEED 100
#define OBJECT_MAX_CHECK_BALL_DISTANCE 9

// These all need to be adjusted.
#define SMALL_INTERVAL_SPEED 6
#define SMALL_INTERVAL_DEGREES 4
#define SMALL_INTERVAL_MAX_DISTANCE 3
#define SMALL_INTERVAL_MAX_CONSECUTIVE 20

#define LARGE_INTERVAL_SPEED 30
#define LARGE_INTERVAL_DEGREES 20

#define MAX_WALL_NEW_COURSE_DISTANCE 19
#define MAX_WALL_FIND_OPENING_DISTANCE 18

#define WALL_CHECK_BACKWARD_DEGREES 50
#define WALL_CHECK_TURN_DEGREES 200
#define WALL_CHECK_TURN_RATIO -1
#define WALL_CHECK_TURN_SPEED 100


// Create the prizm object to control the robot.
PRIZM prizm;


// The following line may be temporary...
bool isLine();


// This function prints output to the serial monitor for observation (debugging).
template<typename T> void print(T line, bool end=true)
{
	if (end)
	{
		Serial.println(line);
	}
	else
	{
		Serial.print(line);
	}
	
}


// When DC motors have been set to move a certain amount, this function can be used to wait for the
// DC motors to stop.
void waitMotorsStop()
{
	while (prizm.readMotorBusy(1) || prizm.readMotorBusy(2)) {isLine();}  // The "isLine();" part may be temporary.
}


// This function immediately stops the DC motors.
void stopMotors()
{
	prizm.setMotorSpeeds(0, 0);
}


// Perform a turn given a speed, certain degrees, a ratio between the two motor speed and degrees,
// and whether the turn is left or right.
void turn(int speed, int degrees, int ratio, bool leftORRight, bool waitStop = true)
{
	prizm.resetEncoders();

	if (leftORRight == LEFT)
	{
		prizm.setMotorDegrees(speed, degrees, abs(speed / ratio), degrees / ratio);
	}
	else
	{
		prizm.setMotorDegrees(abs(speed / ratio), degrees / ratio, speed, degrees);
	}

	if (waitStop)
	{
		waitMotorsStop();
	}
}


// This function moves forward or backward using the above turn function, given a speed, and
// degrees.
void move(int speed, int degrees, bool waitStop = true)
{
	turn(speed, degrees, STRAIGHT_RATIO, true, waitStop);
}


// This function can be used to turn the robot left or right.
void bigTurn(bool leftORRight)
{
	turn(BIG_TURN_SPEED, BIG_TURN_DEGREES, BIG_TURN_RATIO, leftORRight);
}


// This function can be used to "undo" a left or right turn.
void undoBigTurn(bool leftORRight)
{
	turn(BIG_TURN_SPEED, BIG_TURN_DEGREES * -1, BIG_TURN_RATIO, leftORRight);
}


// This function performs a u-turn.
void uTurn()
{
	move(BIG_TURN_SPEED, -200);
	delay(500);
	turn(100, 611, -1, RIGHT);
}


// This function moves the robot to move forward until explicitly stopped.
void moveForward()
{
	prizm.setMotorSpeeds(FORWARD_SPEED, FORWARD_SPEED);
}


// Create an array to keep track of whether the line was visible in the last LINE_CHECK_ITERATIONS
// number of iterations.
bool previousLineVisible[LINE_CHECK_ITERATIONS] = {false};


// This functions resets the above array.
void clearLineData()
{
	for (int i = 0; i < LINE_CHECK_ITERATIONS; i++)
	{
		previousLineVisible[i] = false;
	}
}


// This function returns whether or not the line sensor sees a line.
bool isLine()
{
	bool result;

	// Move the previously results of whether the line was visible one index back each.
	for (int i = 0; i < LINE_CHECK_ITERATIONS - 1; i++)
	{
		previousLineVisible[i] = previousLineVisible[i + 1];
	}

	// At the last index, place whether the line is visible now.
	previousLineVisible[LINE_CHECK_ITERATIONS - 1] = prizm.readLineSensor(LINE_SENSOR_PORT) == 0;


	// Set the result as false initially.
	result = false;

	// Search for any true value, upon finding any true value, the result becomes true;
	for (int j = 0; j < LINE_CHECK_ITERATIONS; j++)
	{
		if (previousLineVisible[j] == true)
		{
			result = true;
			break;
		}
	}

	// Return whether the line is "visible" or not.
	if (result == true && prizm.readStartButton() == 1)
	{
		print("WHY");
	}

	return result;
}


// This function turns the robot left or right until it sees a line.  A maximum turning degrees is
// set.
bool turnToLine(bool leftORRight, bool wide = false)
{
	// If a line appears initially, give the result immediately.
	if (isLine())
	{
		return true;
	}

	if (wide)
	{
		turn(100, 75, -1, leftORRight, false);
	}
	else
	{
		// Make a small turn, left or right, to find a line.
		turn(LINE_TURN_SPEED, LINE_TURN_DEGREES, LINE_TURN_RATIO, leftORRight, false);
		print("not wide");
	}
	
	while (prizm.readMotorBusy(1) || prizm.readMotorBusy(2))
	{
		print("stuck");
		if (isLine())
		{
			// Stop turning immediately upon finding a line.
			stopMotors();

			// Make a small extra turn.
			turn(LINE_TURN_EXTRA_SPEED, LINE_TURN_EXTRA_DEGREES, LINE_TURN_EXTRA_RATIO, leftORRight);
			
			// The line has been found.
			print("made it 1");
			return true;
		}
	}

	// Undo the initial turn made to find a line.
	//turn(LINE_TURN_SPEED, LINE_TURN_DEGREES * -1, LINE_TURN_RATIO, leftORRight);

	if (wide)
	{
		turn(100, -75, -1, leftORRight);
	}
	else
	{
		// Make a small turn, left or right, to find a line.
		turn(LINE_TURN_SPEED, LINE_TURN_DEGREES * -1, LINE_TURN_RATIO, leftORRight);
		print("not wide");
	}

	// The line has not been found.
	print("made it 2");
	return false;
}


// This function may or may not be used in the future.
void turnToObject()
{
	// if (criteria...)
	// {
	// 	return;
	// }

	// turn(LINE_TURN_SPEED, LINE_TURN_DEGREES, LINE_TURN_RATIO, leftORRight, false);
	
	// while (prizm.readMotorBusy(1) || prizm.readMotorBusy(2))
	// {
	// 	if (isLine())
	// 	{
	// 		stopMotors();

	// 		if (prizm.readEncoderDegrees(1) > 1)
	// 		{
	// 			turn(LINE_TURN_EXTRA_SPEED, LINE_TURN_EXTRA_DEGREES, LINE_TURN_EXTRA_RATIO, leftORRight);

	// 		}
			
	// 		return true;
	// 	}
	// }

	// turn(LINE_TURN_SPEED, LINE_TURN_DEGREES * -1, LINE_TURN_RATIO, leftORRight, false);

	// return false;
}


// This function checks for the existence of a line left or right. Blah, blah, blah.
bool teeTurn(bool leftORRight)
{
	bool teeVisible = false; 

	// Move backward and make a small "tee" turn.  Also, clear the line data to avoid confusion.
	move(TEE_TURN_SPEED, TEE_TURN_BACKWARD_DEGREES * -1);
	turn(TEE_TURN_SPEED, TEE_TURN_DEGREES, TEE_TURN_RATIO, leftORRight);

	clearLineData();


	// Check forward for a line.
	move(TEE_TURN_SPEED, TEE_TURN_FORWARD_CHECK_DEGREES, false);

	while (prizm.readMotorBusy(1))
	{
		if (prizm.readLineSensor(LINE_SENSOR_PORT) == 0)
		{
			teeVisible = true;
			break;
		}
	}

	waitMotorsStop();

	// Undo the forward line check movement, the leftORRight or right turn and the initial backward
	// movement.
	move(TEE_TURN_SPEED, TEE_TURN_FORWARD_CHECK_DEGREES * -1);
	turn(TEE_TURN_SPEED, TEE_TURN_DEGREES * -1, TEE_TURN_RATIO, leftORRight);
	move(TEE_TURN_SPEED, TEE_TURN_BACKWARD_DEGREES);

	// Return whether a line has been found.
	return teeVisible;
}


// Define a data type to store the state observed upon a left and right tee turn.
enum LineState {noLine, tee, rightTurn, leftTurn};


// This function makes a left and right tee turn and returns the "state" i.e. what has to be done.
LineState checkLineState()
{
	bool teeRight = teeTurn(false), teeLeft = teeTurn(true);

	return teeRight ? (teeLeft ? tee : rightTurn) : (teeLeft ? leftTurn : noLine);
}


// This function opens the claw.
void openClaw()
{
	prizm.setServoPosition(CLAW_PORT, CLAW_OPEN_DEGREES);
	delay(CLAW_TIME);
}


// This function closes the claw.
void closeClaw()
{
	prizm.setServoPosition(CLAW_PORT, CLAW_CLOSE_DEGREES);
	delay(CLAW_TIME);
}


// This function sets the arm at a specific height (degrees).
void changeArmHeight(unsigned short degrees)
{
	prizm.setServoPosition(ARM_PORT, degrees);
	delay(ARM_TIME);
}


// Use an ultrasonic sensor to read in a distance.  This function takes the average of 100 values
// discards values that do not make sense.
unsigned short getObjectDistance(int sensorPort, int maxDistance)
{
	// unsigned int sum = 0;
	// unsigned short count = 0;
	// unsigned short reading;

	// for (int i = 0; i < SONIC_READING_COUNT; i++)
	// {
	// 	reading = prizm.readSonicSensorCM(sensorPort);
		
	// 	if (reading <= maxDistance)
	// 	{
	// 		count++;
	// 		sum += reading;
	// 	}

	// 	// delay(10);
	// }

	// return count > 0 ? round((double)sum / count) : 0;
	for (int i = 0; i < 5; i++)
	{
		int reading = prizm.readSonicSensorCM(sensorPort);
		if (reading < maxDistance)
		{
			return reading;
		}
	}

	return 0;
}


// Given a sensor port and ideal distance, this function will adjust the robot so that it lies
// a specific number of centimeters in front of an object.
void adjustObjectDistance(int sensorPort, int idealDistance, int maxDistance)
{
	// Get the initial distance between the robot and the ball.
	int currentDistance = getObjectDistance(sensorPort, maxDistance);

	print("Distance:");
	print(currentDistance);
	print("");

	short direction, consecutiveSmallIntervals = 0;

	// Move forward or backward until the ideal distance has been reached.
	while (currentDistance != idealDistance && consecutiveSmallIntervals <= SMALL_INTERVAL_MAX_CONSECUTIVE)
	{
		Serial.println(currentDistance);

		// Set the direction of the robot based on the distance to the object.
		direction = currentDistance < idealDistance ? -1 : 1;

		// Move a long or short distance based on how near or far the object is.
		if (abs(currentDistance - idealDistance) > SMALL_INTERVAL_MAX_DISTANCE)
		{
			move(LARGE_INTERVAL_SPEED, LARGE_INTERVAL_DEGREES * direction);
			consecutiveSmallIntervals = 0;
		}
		else
		{
			move(SMALL_INTERVAL_SPEED, SMALL_INTERVAL_DEGREES * direction);
			consecutiveSmallIntervals++;
		}

		// Get the new distance from the object.
		currentDistance = getObjectDistance(sensorPort, maxDistance);
	
		print("Distance:");
		print(currentDistance);
		print("");
	}
}


// This function still has to be made.
bool isObjectFront(int sensorPort, unsigned int maxDistance, unsigned int checkDistance)
{
	unsigned int distance = getObjectDistance(sensorPort, maxDistance);

	if (distance > checkDistance || distance == 0)
	{
		print("There is no object.");
		print(distance);
		return false;
	}
	else
	{
		print("Yes, there is an object.");
		print(distance);
		return true;
	}
}


// This function sets the claw at a specific height to check for the ball's height: high or low.
bool objectHighOrLow()
{
	changeArmHeight(ARM_BALL_CHECK_DEGREES);

	return isObjectFront(CLAW_SONIC_SENSOR_PORT, OBJECT_MAX_DISTANCE, OBJECT_MAX_CHECK_BALL_DISTANCE) ? HIGH : LOW;	
}


// This function will turn the robot both way to check which way to turn, by detecting the walls.
int checkWalls()
{
	// Move backward, lower the arm, and turn left.
	move(WALL_CHECK_TURN_SPEED, -WALL_CHECK_BACKWARD_DEGREES);
	bigTurn(LEFT);
	delay(100);
	move(100, 30);

	delay(500);

	// Report readings (just a test).
	// for (int i = 0; i < 5; i++)
	// {
	// 	print("RUNNING TEST OF READINGS...");
	// 	isObjectFront(STATIC_SONIC_SENSOR_PORT, WALL_MAX_DISTANCE, MAX_WALL_FIND_OPENING_DISTANCE);
	// 	delay(1000);
	// 	print("COMPLETED TEST");
	// }

	if (isObjectFront(STATIC_SONIC_SENSOR_PORT, WALL_MAX_DISTANCE, MAX_WALL_FIND_OPENING_DISTANCE))
	{
		delay(100);

		move(100, -30);

		// A wall has been found on the left side, continue to check for the right side.
		undoBigTurn(LEFT);

		bigTurn(RIGHT);

		move(100, 30);

		delay(500);

		if (isObjectFront(STATIC_SONIC_SENSOR_PORT, WALL_MAX_DISTANCE, MAX_WALL_FIND_OPENING_DISTANCE))
		{
			delay(100);

			move(100, -30);

			delay(100);


			//  A wall has been found on the right side.  The program has completed!
			undoBigTurn(RIGHT);

			return STOP;
		}
		else
		{
			return RIGHT;
		}
	}
	else
	{
		return LEFT;
	}
	
	// The robot has found an opening and must proceed.
}


// Once the robot is turned in the correct direction, this function will move the robot forward
// until it sees a line and a wall in range.  It stops, then turns left.
void moveToNextCourse(int previousWallState)
{
	move(100, -50);
	turn(100, 60, -1, !previousWallState);
	delay(100);
	move(200, 200);
	delay(100);

	turn(100, 60, -1, previousWallState);
	delay(100);
	move(200, 200);

	
	int wallState = checkWalls();

	if (wallState == STOP)
	{
		prizm.PrizmEnd();
	}

	undoBigTurn(wallState);

	delay(200);

	turn(100, 55, -1, wallState);
	delay(200);

	move(200, 50);
	changeArmHeight(ARM_HIGH_BALL_DEGREES);

	moveForward();

	int count = 0;
	while (1)
	{
		if (isObjectFront(CLAW_SONIC_SENSOR_PORT, WALL_MAX_DISTANCE, MAX_WALL_NEW_COURSE_DISTANCE))
		{
			if (prizm.readLineSensor(LINE_SENSOR_PORT) == 0)
			{
				print("Line, line, line.");
				count++;
				if (count > 0)
				{
					break;
				}
			}

			print("Object is up front.");
		}
		else
		{
			//count = 0;
		}

		delay(10);
	}

	move(100, 10);
	delay(100);

	turn(100, 55, -1, !wallState);

	bigTurn(wallState);


	// delay(165);

	// stopMotors();

	// changeArmHeight(ARM_UP_DEGREES);

	// bigTurn(LEFT);

	// move(200, -100);
}


// This function initializes the robot by setting a few values, adjusting motor positions, etc.
void setupRobot()
{
	prizm.PrizmBegin();
	prizm.setMotorInvert(2, 1);
	prizm.setServoSpeed(CLAW_PORT, CLAW_SPEED);
	prizm.setServoSpeed(ARM_PORT, ARM_SPEED);
	Serial.begin(9600);
}

#endif