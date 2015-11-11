#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    ExternalBatteryValue, sensorAnalog)
#pragma config(Sensor, in2,    LauncherPosition, sensorPotentiometer)
#pragma config(Sensor, in3,    BallDispenserPosition, sensorPotentiometer)
#pragma config(Sensor, dgtl3,  BallLoaded,     sensorTouch)
#pragma config(Sensor, dgtl4,  BallReleased,     sensorTouch)
#pragma config(Sensor, I2C_1,  FrontLeft_I2C,  sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           Belt,          tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FrontLeft,     tmotorVex393_MC29, openLoop, reversed, driveLeft, encoderPort, None)
#pragma config(Motor,  port3,           FrontRight,    tmotorVex393_MC29, openLoop, driveRight, encoderPort, I2C_1)
#pragma config(Motor,  port4,           Launcher1,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           Launcher2,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           Launcher3,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           Launcher4,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           BackLeft,      tmotorVex393_MC29, openLoop, reversed, driveLeft)
#pragma config(Motor,  port9,           BackRight,     tmotorVex393_MC29, openLoop, driveRight, encoderPort, None)
#pragma config(Motor,  port10,          BallDispenser, tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

task LauncherUp();
task LauncherStop();
task LauncherDown();
task LaunchBall();
task StopBelt();
task DispenseBall();
task OpenDispenser();
task CloseDispenser();
task MoveBeltToReadyBall();
task AutoLaunchBall();
task displayBatteryLevelOnLCD();

// Global variables

// For a new rubber band
//int powerToDownLauncher = 76;
//int powerToLaunch = 81;
//int powerToStay = 19;

enum GameMode{
	Far = 0,
	Middle = 1,
	Near = 2,
	Farthest = 3,
};


// For a used rubber band
GameMode LauncherRange = Far;
int powerToDown = 70;
int postionToDown = 1380;
int powerToStay = 18;

int powerToLaunch = 92;
int positionToStop = 1480;

int powerToLaunch_Farthest = 99;
int positionToStop_Farthest = 1480;

int powerToLaunch_Mid = 95;
int positionToStop_Mid = postionToDown - 100;
//int positionToStop_Mid = postionToDown + 50;

int powerToLaunch_Short = 73;
int positionToStop_Short = 155;

//int postionToDown = 1280;
//int postionToDown = 2900;

// Position to break launcher



void ForBack(int originalPower, int distance);
void EncoderRotate(int power, int distance);
void StopMoving();
void StopOrReverseBelt();
int AdjustPowerUsingBatteryLevel(int originalPower);
int AdjustPowerUsingExternalBatteryLevel(int originalPower);


void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
	// .....................................................................................
	// Insert user code here.
	// .....................................................................................

	writeDebugStreamLine("(autonomous started) Time: %d, %d", nPgmTime, nSysTime);

	int i = 0;
	while(true)
	{
		// Repeat for 4 times (4 preloads)
		if (i > 3)
		{
			break;
		}

		// move backward to aligh balls
		motor[Belt] = -70;
		wait1Msec(150);
		motor[Belt] = 0;


		//int launcherPosition = SensorValue[LauncherPosition];
		//writeDebugStreamLine("LaunchBall) launcherPosition :%d", launcherPosition);

		// DOwn the launcher
		int originalPower = powerToDown; // power to down the launcher
		int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
		int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
		writeDebugStreamLine("LauncherUp) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

		motor[Launcher1] = externalPower;
		motor[Launcher2] = externalPower;
		motor[Launcher3] = primaryPower;
		motor[Launcher4] = primaryPower;
		//motor[Launcher5] = power;
		//motor[Launcher6] = power;

		// Down until it is lower than the position (1055)
		clearTimer(T3);
		while(SensorValue[LauncherPosition] > postionToDown && time1[T3] < 1000)
		{
		}

		// Down and stay waiting a ball loaded
		originalPower = powerToStay;
		primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
		externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
		writeDebugStreamLine("LauncherUp) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

		motor[Launcher1] = externalPower;
		motor[Launcher2] = externalPower;
		motor[Launcher3] = primaryPower;
		motor[Launcher4] = primaryPower;
		//motor[Launcher5] = power;
		//motor[Launcher6] = power;

		// Open the dispenser
		startTask(OpenDispenser);

		// Move the fall forward to make a ball ready
		startTask(MoveBeltToReadyBall);

		// Wait until a ball loaded
		clearTimer(T3);
		while(SensorValue[BallLoaded] == 0 && time1[T3] < 2000)
		{
		}

		// Close the dispenser
		startTask(CloseDispenser);

		//if (i == 0)
		//{
		//	wait1Msec(200);
		//motor[Belt] = 0;
		//}
		// Timeout and move the launcher to the original position
		if(time1[T3] >= 2000)
		{
			//startTask(LauncherUp);
			startTask(LauncherStop);
		}
		else
		{
			// Ball is loaded, launch the ball
			wait1Msec(1300);
			startTask(LauncherUp);

			//wait1Msec(150);
			//startTask(LauncherStop);

			// Increase
			i++;
		}
		// Delay for 1 sec before the nexr launch
		wait1Msec(500);
	}

	writeDebugStreamLine("(autonomous stopped) Time: %d, %d", nPgmTime, nSysTime);

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
task displayBatteryLevelOnLCD()
{
	bLCDBacklight=true;

	//while(true)                                                        // An infinite loop to keep the program running until you terminate it
	//{
	clearLCDLine(0);                                            // Clear line 1 (0) of the LCD
	clearLCDLine(1);                                            // Clear line 2 (1) of the LCD

	string mainBattery, externalBattery;

	//Display the Primary Robot battery voltage
	displayLCDString(0, 0, "Primary: ");
	sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
	displayNextLCDString(mainBattery);

	//float externalBatteryLevel = SensorValue[ExternalBatteryValue];
	//Display the Backup battery voltage
	displayLCDString(1, 0, "External: ");
	sprintf(externalBattery, "%1.2f%c", (SensorValue[ExternalBatteryValue] * 3.57)/1000.0, 'V');    //Build the value to be displayed
	displayNextLCDString(externalBattery);

	//Short delay for the LCD refresh rate
	wait1Msec(700);
	//}

	bLCDBacklight=false;
}

int AdjustPowerUsingBatteryLevel(int originalPower)
{
	float batteryLevel = nImmediateBatteryLevel;
	float batteryOffset =	7000 / batteryLevel;
	int adjustedPower = originalPower * batteryOffset;
	//  writeDebugStreamLine("(AjustBattery) BatterLevel: %d - OriginalPower: %d - WantedPower: %d", AdjustBatteryLevel, OriginalPower, WantedPower);
	return adjustedPower;
}

int AdjustPowerUsingExternalBatteryLevel(int originalPower)
{
	float batteryLevel = SensorValue[ExternalBatteryValue] * 3.57;

	float batteryOffset =	7000 / batteryLevel;
	int adjustedPower = originalPower * batteryOffset;
	//  writeDebugStreamLine("(AjustBattery) BatterLevel: %d - OriginalPower: %d - WantedPower: %d", AdjustBatteryLevel, OriginalPower, WantedPower);
	return adjustedPower;
}

task LauncherUp_Skill()
{
	startTask(LauncherStop);

	// 1 rubber band and short
	int originalPower = powerToLaunch + 1;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower) * -1;
	writeDebugStreamLine("LauncherUp) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;
	//motor[Launcher5] = power;
	//motor[Launcher6] = power;

	wait1Msec(200);

	startTask(LauncherStop);
}

task LauncherUp()
{
	startTask(LauncherStop);

	//int positionToStop = 1480;

	int originalPower = powerToLaunch;
	int originalPower_external = powerToLaunch;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(SensorValue[LauncherPosition] < positionToStop && time1[T1] < 500)
	{
	}

	if (time1[T1] > 500)
	{
		startTask(LauncherStop);
		return;
	}

	startTask(LauncherStop);
	//int power1 = 0;
	//motor[Launcher1] = power1;
	//motor[Launcher2] = power1;
	//motor[Launcher3] = power1;
	//motor[Launcher4] = power1;
}

task LauncherUp_Farthest()
{
	startTask(LauncherStop);

	//int positionToStop = 1480;

	int originalPower = powerToLaunch_Farthest;
	int originalPower_external = powerToLaunch_Farthest;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(SensorValue[LauncherPosition] < positionToStop_Farthest && time1[T1] < 500)
	{
	}

	if (time1[T1] > 500)
	{
		startTask(LauncherStop);
		return;
	}

	startTask(LauncherStop);
	//int power1 = 0;
	//motor[Launcher1] = power1;
	//motor[Launcher2] = power1;
	//motor[Launcher3] = power1;
	//motor[Launcher4] = power1;
}

task LauncherUp_Mid()
{
	startTask(LauncherStop);

	//startTask(LauncherStop);
	//waitUntilMotorStop(motor[Launcher1]);

	//wait1Msec(100);

	// 1 rubber band and short

	//int positionToStop = 1480;

	int originalPower = powerToLaunch_Mid;
	int originalPower_external = powerToLaunch_Mid;

	//int originalPower2 = 40;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Mid) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(SensorValue[LauncherPosition] < (positionToStop_Mid) && time1[T1] < 500)
	{
	}

	if (time1[T1] > 500)
	{
		startTask(LauncherStop);
		return;
	}

	//waitUntil(motor[Launcher1] == externalPower && motor[Launcher2] == externalPower && motor[Launcher3] == primaryPower && motor[Launcher4] == primaryPower);
	//wait1Msec(55);

	//writeDebugStreamLine("LauncherUp_Short) timer1:%f", time1[T1]);

	int extPower = 20;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(100);

	startTask(LauncherStop);
}


task LauncherUp_Short()
{
	startTask(LauncherStop);

	//startTask(LauncherStop);
	//waitUntilMotorStop(motor[Launcher1]);

	//wait1Msec(100);

	// 1 rubber band and short

	//int positionToStop = 1480;

	int originalPower = powerToLaunch_Mid - 55;
	int originalPower_external = powerToLaunch_Mid;

	//int originalPower2 = 40;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Mid) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(SensorValue[LauncherPosition] < (positionToStop_Mid) && time1[T1] < 500)
	{
	}

	if (time1[T1] > 500)
	{
		startTask(LauncherStop);
		return;
	}

	//waitUntil(motor[Launcher1] == externalPower && motor[Launcher2] == externalPower && motor[Launcher3] == primaryPower && motor[Launcher4] == primaryPower);
	//wait1Msec(55);

	//writeDebugStreamLine("LauncherUp_Short) timer1:%f", time1[T1]);

	int extPower = 20;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(100);

	startTask(LauncherStop);
}



task LauncherUp_Short_v2()
{
	startTask(LauncherStop);

	int originalPower = powerToLaunch_Short;
	int originalPower_external = powerToLaunch_Short;

	//int pos = SensorValue[LauncherPosition];

	////if (pos > 1169)
	//if (pos > 1166)
	//{
	//	startTask(LauncherStop);
	//	wait1Msec(15);
	//}
	//int originalPower2 = 40;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	//writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);
	//writeDebugStreamLine("LauncherUp_Short) pos :%d", pos);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(time1[T1] < 60)
	{
	}

	//waitUntil(motor[Launcher1] == externalPower && motor[Launcher2] == externalPower && motor[Launcher3] == primaryPower && motor[Launcher4] == primaryPower);
	//wait1Msec(55);

	//writeDebugStreamLine("LauncherUp_Short) timer1:%f", time1[T1]);

	int extPower = 15;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(100);

	startTask(LauncherStop);
}


task LauncherUp_Short_new_old()
{
	startTask(LauncherStop);

	int originalPower = powerToLaunch_Short;
	int originalPower_external = powerToLaunch_Short;

	int pos = SensorValue[LauncherPosition];

	//if (pos > 1169)
	if (pos > 1166)
	{
		startTask(LauncherStop);
		wait1Msec(15);
	}
	//int originalPower2 = 40;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	//writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);
	writeDebugStreamLine("LauncherUp_Short) pos :%d", pos);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(SensorValue[LauncherPosition] < (pos + 50 /*positionToStop_Short*/) && time1[T1] < 500)
	{
	}

	if (time1[T1] > 500)
	{
		startTask(LauncherStop);
		return;
	}

	//waitUntil(motor[Launcher1] == externalPower && motor[Launcher2] == externalPower && motor[Launcher3] == primaryPower && motor[Launcher4] == primaryPower);
	//wait1Msec(55);

	//writeDebugStreamLine("LauncherUp_Short) timer1:%f", time1[T1]);

	int extPower = 20;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(100);

	startTask(LauncherStop);
}

task LauncherUp_Short_Old()
{
	startTask(LauncherStop);

	//startTask(LauncherStop);
	//waitUntilMotorStop(motor[Launcher1]);

	//wait1Msec(100);

	// 1 rubber band and short

	//int positionToStop = 1480;

	int originalPower = powerToLaunch_Short;
	int originalPower_external = powerToLaunch_Short;

	int pos = SensorValue[LauncherPosition];

	//int originalPower2 = 40;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(SensorValue[LauncherPosition] < (pos + positionToStop_Short) && time1[T1] < 500)
	{
	}

	if (time1[T1] > 500)
	{
		startTask(LauncherStop);
		return;
	}

	//waitUntil(motor[Launcher1] == externalPower && motor[Launcher2] == externalPower && motor[Launcher3] == primaryPower && motor[Launcher4] == primaryPower);
	//wait1Msec(55);

	//writeDebugStreamLine("LauncherUp_Short) timer1:%f", time1[T1]);

	int extPower = 20;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(100);

	startTask(LauncherStop);
}

task LauncherStop()
{
	//int originalPower = -10;
	//int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	//int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);

	//motor[Launcher1] = externalPower;
	//motor[Launcher2] = externalPower;
	//motor[Launcher3] = primaryPower;
	//motor[Launcher4] = primaryPower;

	//int positionToStop = 2000;

	//clearTimer(T1);
	//while(SensorValue[LauncherPosition] < (positionToStop) && time1[T1] < 500)
	//{
	//}

	//int power = 0;
	//motor[Launcher1] = power;
	//motor[Launcher2] = power;
	//motor[Launcher3] = power;
	//motor[Launcher4] = power;
	//wait1Msec(20);

	//originalPower = 20;
	//primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	//externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);

	//motor[Launcher1] = externalPower;
	//motor[Launcher2] = externalPower;
	//motor[Launcher3] = primaryPower;
	//motor[Launcher4] = primaryPower;


	int power = 0;
	//motor[Launcher1] = power;
	//motor[Launcher2] = power;
	//motor[Launcher3] = power;
	//motor[Launcher4] = power;

	//wait1Msec(50);

	//power = 0;
	motor[Launcher1] = power;
	motor[Launcher2] = power;
	motor[Launcher3] = power;
	motor[Launcher4] = power;

	//motor[Launcher5] = power;
	//motor[Launcher6] = power;
}


task LauncherDown()
{// T3
	//int power = 70; // perfect but strong

	int originalPower = powerToDown;
	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("LauncherDown) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;
	//motor[Launcher5] = power;
	//motor[Launcher6] = power;

	clearTimer(T3);
	while(SensorValue[LauncherPosition] > postionToDown && time1[T3] < 1000)
	{
	}

	originalPower =  powerToStay;
	primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("LauncherDown) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	//clearTimer(T3);
	//while(time1[T3] < 3000)
	//{
	//}

	//startTask(LauncherStop);

	clearTimer(T3);
	while(SensorValue[BallLoaded] == 0 && time1[T3] < 2000)
	{
	}
	if(time1[T3] >= 2000)
	{
		startTask(LauncherStop);
	}
	else
	{
		wait1Msec(300);

		//startTask(LauncherUp_Short);

		if (LauncherRange == Near)
		{
			startTask(LauncherUp_Short);
		}
		else if (LauncherRange == Middle)
		{
			startTask(LauncherUp_Mid);
		}
		else if (LauncherRange == Farthest)
		{
			startTask(LauncherUp_Farthest);
		}
		else
		{
			startTask(LauncherUp);
		}
	}
}

task LaunchBall()
{ // Skills
	while(true)
	{
		stopTask(LauncherUp);
		startTask(LauncherStop);

		//int launcherPosition = SensorValue[LauncherPosition];
		//writeDebugStreamLine("LaunchBall) launcherPosition :%d", launcherPosition);

		int originalPower = powerToDown /* + 3*/; // power to down the launcher
		int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
		int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
		writeDebugStreamLine("LaunchBall) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

		motor[Launcher1] = externalPower;
		motor[Launcher2] = externalPower;
		motor[Launcher3] = primaryPower;
		motor[Launcher4] = primaryPower;
		//motor[Launcher5] = power;
		//motor[Launcher6] = power;

		clearTimer(T3);
		while(SensorValue[LauncherPosition] > postionToDown /* - 10*/ && time1[T3] < 1000)
		{
		}

		originalPower = powerToStay;
		primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
		externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
		writeDebugStreamLine("LaunchBall) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

		motor[Launcher1] = externalPower;
		motor[Launcher2] = externalPower;
		motor[Launcher3] = primaryPower;
		motor[Launcher4] = primaryPower;
		//motor[Launcher5] = power;
		//motor[Launcher6] = power;

		clearTimer(T3);
		while(SensorValue[BallLoaded] == 0 && time1[T3] < 1000)
		{
		}
		if(time1[T3] >= 1000)
		{
			//startTask(LauncherUp);
			startTask(LauncherStop);

		}
		else
		{
			wait1Msec(330); // Skill
			//wait1Msec(500); // Drive
			if (LauncherRange == Farthest)
			{
				startTask(LauncherUp_Farthest);
			}
			else
			{
				startTask(LauncherUp);
			}
			//startTask(LauncherUp_Skill);

			//wait1Msec(150);
			//startTask(LauncherStop);

		}
		wait1Msec(300); // Skill
		//wait1Msec(300); // Drive
	}
}


void ForBack(int originalPower, int distance)
{
	//motor[FrontLeft] = power;
	//motor[FrontRight] = power;
	//motor[BackRight] = power;
	//motor[BackLeft] = power;

	//wait1Msec(distance);

	float power = originalPower;

	if (power == 0) return;

	//int startPower = 30;
	//	motor[FrontLeft] = (power > 0)? startPower: -startPower; // + (power * 5) / 100;
	//	motor[BackRight] = (power > 0)? startPower: -startPower; // + (power * 5) / 100;

	//	wait1Msec(100);

	int current = abs(nMotorEncoder(FrontRight));
	float offset = 0;

	// RED only for programming skill
	//power = -power;
	float distanceMid = distance / 2;
	float powerMid = power * 1 / 3;

	while (current + offset < distance)
	{
		if (current + offset > distanceMid)
		{
			power = powerMid;
		}

		int previous = current;

		//writeDebugStreamLine("ForBack) power: %f, current + offset: %f, distance: %f, distanceMid: %f", power, current + offset, distance, distanceMid);

		motor[FrontLeft] = power  + (power * 0.1);
		motor[FrontRight] = power + (power * 0.1);
		motor[BackRight] = power ;
		motor[BackLeft] = power;


		writeDebugStreamLine("ForBack) power: %d, current + offset: %f, FrontLeft: %d, FrontRight: %d, BackLeft: %d, BackRight: %d", power, current + offset, motor[FrontLeft], motor[FrontRight], motor[BackLeft], motor[BackRight]);


		int current = abs(nMotorEncoder(FrontRight));

		offset = current - previous;
		offset = offset * 1.2;
	}


	StopMoving();	
	int startPower = 30;
motor[FrontLeft] = (power > 0)? startPower: -startPower; // + (power * 5) / 100;
motor[BackRight] = (power > 0)? startPower: -startPower; // + (power * 5) / 100;

	wait1Msec(200);

	StopMoving();
}


void EncoderRotate(int power, int distance)
{
	int current = abs(nMotorEncoder(FrontRight));
	float offset = 0;

	if (power == 0) return;

	//power = -power;
	while (current + offset < distance)
	{
		int previous = current;

		motor[FrontLeft] = power;
		motor[FrontRight] = -power;
		motor[BackRight] = -power;
		motor[BackLeft] = power;

		int current = abs(nMotorEncoder(FrontRight));

		offset = current - previous;
		offset = offset * 1.6;
	}

	StopMoving();
}

void StopMoving()
{
	motor[FrontLeft] = 0;
	motor[FrontRight] = 0;
	motor[BackRight] = 0;
	motor[BackLeft] = 0;
}

task usercontrol()
{
	// User control code here, inside the loop
	LauncherRange = Far;

	while (true)
	{
		wait1Msec(50);

		int btnLCD = nLCDButtons;
		if (btnLCD >= 2)
		{
			startTask(displayBatteryLevelOnLCD);
		}
		else if (btnLCD == 1)
		{
			stopTask(displayBatteryLevelOnLCD);
		}


		int btn5u = vexRT[Btn5U]; // start belt
		int btn5d = vexRT[Btn5D]; // start belt

		int btn6u = vexRT[Btn6U]; // start belt
		int btn6d = vexRT[Btn6D]; // stop belt

		int btn7u = vexRT[Btn7U]; // Fast Launcher
		int btn7d = vexRT[Btn7D]; // Fast Launcher

		// Launcher Speed
		int btn8u = vexRT[Btn8U]; // Fast Launcher
		int btn8d = vexRT[Btn8D]; // Slow Launcher
		int btn8l = vexRT[Btn8L]; // Slow Launcher
		int btn8r = vexRT[Btn8R]; // Slow Launcher

		int power3 = - vexRT[Ch3]; // forward, backward
		int power1 = vexRT[Ch1]; // rotate
		if (power1 > 50)
		{
			power1 = power1 * 90 / 100;
		}
		int power7 = 70; // shift
		int btn7r = vexRT[Btn7R]; // shift
		int btn7l = vexRT[Btn7L]; // shift

		//standard drive motor block
		motor[FrontLeft] = power3 - power1 - btn7r * power7 + btn7l * power7;
		motor[FrontRight] = (power3 + power1 + btn7r * power7 - btn7l * power7);//* 90 / 100;
		motor[BackRight] = (power3 + power1 - btn7r * power7 + btn7l * power7);// * 90 / 100;
		motor[BackLeft] = (power3 - power1 + btn7r * power7 - btn7l * power7);// * 90 / 100;


		if (btn8u == 1)
		{
			//nMotorEncoder(FrontRight) = 0;
			//ForBack(-80, 1000);

			stopTask(LauncherDown);

			wait1Msec(10);

			if (LauncherRange == Near)
			{
				startTask(LauncherUp_Short);
			}
			else if (LauncherRange == Middle)
			{
				startTask(LauncherUp_Mid);
			}
			else if (LauncherRange == Farthest)
			{
				startTask(LauncherUp_Farthest);
			}
			else
			{
				startTask(LauncherUp);
			}
		}
		else if (btn8d == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = Farthest;
			startTask(AutoLaunchBall);

			//LauncherRange = Near;

			//stopTask(LauncherUp);
			//startTask(LauncherDown);

		}
		else if (btn8l == 1)
		{

			startTask(LaunchBall);
		}
		else if (btn8r == 1)
		{

			//nMotorEncoder(FrontRight) = 0;
			//EncoderRotate(50, 250);

			// Open or Close the ball dispenser
			if (SensorValue[BallDispenserPosition] >= 1300)
			{
				// Open
				//stopTask(CloseDispenser);
				//startTask(OpenDispenser);
				motor[BallDispenser] = 75;

				clearTimer(T2);
				while(SensorValue[BallDispenserPosition] > 1047 && time1[T2] < 5000)
				{
				}

				motor[BallDispenser] = 0;
			}
			else
			{	// Close
				//stopTask(OpenDispenser);
				//startTask(CloseDispenser);

				motor[BallDispenser] = -75;

				clearTimer(T2);
				while(SensorValue[BallDispenserPosition] < 1630 && time1[T2] < 5000)
				{
				}

				motor[BallDispenser] = 0;

			}

		}
		else if (btn6u == 1)
		{
			motor[Belt] = 80;
		}
		else if (btn6d == 1)
		{
			//motor[Belt] = 0;

			StopOrReverseBelt();
		}
		else if (btn5u == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = Near;
			startTask(AutoLaunchBall);
			//startTask(DispenseBall);
		}
		else if (btn7u == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = Middle;
			startTask(AutoLaunchBall);
			//startTask(DispenseBall);
		}
		else if (btn7d == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = Far;
			startTask(AutoLaunchBall);
			//startTask(DispenseBall);
		}
		else if (btn5d == 1)
		{
			motor[Belt] = 0;
			stopTask(LauncherUp);
			stopTask(LauncherDown);
			stopTask(LaunchBall);
			stopTask(AutoLaunchBall);
			startTask(LauncherStop);
			startTask(CloseDispenser);
		}
	}
}

void StopOrReverseBelt()
{

	wait1Msec(200);
	int power = motor[Belt];

	if (power != 0)
	{
		startTask(StopBelt);
	}
	else
	{
		motor[Belt] = -40;
		wait1Msec(150);
	}
}

task StopBelt()
{
	int power = 0;
	motor[Belt] = power;
}

task DispenseBall()
{
	motor[Belt] = 0;
	wait1Msec(100);

	motor[Belt] = -80;
	wait1Msec(100);
	motor[Belt] = 0;


	startTask(OpenDispenser);

	wait1Msec(50);

	motor[Belt] = 80;

	startTask(CloseDispenser);

	wait1Msec(1600);
	motor[Belt] = 0;
}

task OpenDispenser()
{// T2
	motor[BallDispenser] = 75;

	clearTimer(T2);
	while(SensorValue[BallDispenserPosition] > 1047 && time1[T2] < 5000)
	{
	}

	motor[BallDispenser] = 0;

}

task CloseDispenser()
{// T2
	//clearTimer(T2);
	//while(SensorValue[BallLoaded] == 0 && time1[T2] < 3000)
	//{
	//}

	motor[BallDispenser] = -75;

	clearTimer(T2);
	while(SensorValue[BallDispenserPosition] < 1630 && time1[T2] < 5000)
	{
	}

	motor[BallDispenser] = 0;
}

task AutoLaunchBall()
{
	// T2
	startTask(CloseDispenser);

	motor[Belt] = -100;
	wait1Msec(150);
	//wait1Msec(200);
	motor[Belt] = 0;

	//int launcherPosition = SensorValue[LauncherPosition];
	//writeDebugStreamLine("LaunchBall) launcherPosition :%d", launcherPosition);

	int originalPower = powerToDown; // power to down the launcher
	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("AutoLaunchBall) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T3);
	while(SensorValue[LauncherPosition] > postionToDown && time1[T3] < 1000)
	{
	}

	originalPower = powerToStay;
	primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("AutoLaunchBall) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	startTask(OpenDispenser);

	startTask(MoveBeltToReadyBall);

	clearTimer(T3);
	bool ballReleased = false;
	bool ballLoaded = false;

	while(SensorValue[BallReleased] == 0 && SensorValue[BallLoaded] == 0 && time1[T3] < 3000)
	{
	}


	if (SensorValue[BallReleased] > 0)
	{

		wait1Msec(150);

		ballReleased = true;
	}


	//if (SensorValue[BallLoaded] > 0)
	//{
	//	BallLoaded = true;
	//}

	startTask(CloseDispenser);


	if(time1[T3] >= 3000)
	{
		startTask(LauncherStop);
	}
	else if (ballReleased == true)
	{
		while(SensorValue[BallLoaded] == 0 && time1[T3] < 3000)
		{
		}
	}

	if (SensorValue[BallLoaded] == 1)
	{
		//wait1Msec(500);
		wait1Msec(400);

		if (LauncherRange == Near)
		{
			startTask(LauncherUp_Short);
		}
		else if (LauncherRange == Middle)
		{
			startTask(LauncherUp_Mid);
		}
		else if (LauncherRange == Farthest)
		{
			startTask(LauncherUp_Farthest);
		}
		else
		{
			startTask(LauncherUp);
		}
		//wait1Msec(200);
		//startTask(LauncherStop);
	}
	else
	{
		startTask(LauncherStop);
	}


}

task MoveBeltToReadyBall()
{// T4
	motor[Belt] = 60;

	clearTimer(T4);
	while(time1[T4] < 2000)
	{
	}

	motor[Belt] = 0;

}
