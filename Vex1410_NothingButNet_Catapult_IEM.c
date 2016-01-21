#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    ExternalBatteryValue, sensorAnalog)
#pragma config(Sensor, in2,    BallDispenserPosition, sensorPotentiometer)
#pragma config(Sensor, in3,    LauncherPosition, sensorPotentiometer)
#pragma config(Sensor, dgtl3,  BallLoaded,     sensorTouch)
#pragma config(Sensor, dgtl4,  BallReleased,   sensorTouch)
#pragma config(Sensor, dgtl5,  LauncherReady,  sensorTouch)
#pragma config(Sensor, dgtl7,  SonarSensor,    sensorSONAR_mm)
#pragma config(Sensor, dgtl10, Led1,           sensorLEDtoVCC)
#pragma config(Sensor, dgtl12, Jumper1,        sensorDigitalIn)
#pragma config(Sensor, I2C_1,  Launcher_I2C,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  FrontLeft_I2C,  sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           Belt,          tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           FrontLeft,     tmotorVex393_MC29, openLoop, reversed, driveLeft, encoderPort, I2C_2)
#pragma config(Motor,  port3,           FrontRight,    tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port4,           Launcher1,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           Launcher2,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           Launcher3,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           Launcher4,     tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
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
task LaunchBall_ProgrammingSkill();
task StopBelt();
//task DispenseBall();
task OpenDispenser();
task CloseDispenser();
task MoveBeltToReadyBall();
task MoveBeltToReadyFirstBall();
task AutoLaunchBall();
task displayBatteryLevelOnLCD();
task StartBelt();
task StopAutonomous();

// Global variables

// For a new rubber band
//int powerToDownLauncher = 76;
//int powerToLaunch = 81;
//int powerToStay = 19;

enum GameMode{
	Long = 0,
	LongMid = 1,
	Middle = 2,
	Near = 3,
	Longest = 4,
};

int powerForDispenser = 80;
// For a used rubber band
GameMode LauncherRange = Long;

int globalWaiter = 179;
int powerToDown = 80;
int postionToDown = 60;
int powerToStay = 19;

int powerToLaunch = 87;
int positionToStop = 43;

int powerToLaunch_Longest = 95;
//int positionToStop_Longest = 1390;

int powerToLaunch_LongMid = 78;
int positionToStop_LongMid = 35;

//int powerToLaunch_LongMid = 78;
//int positionToStop_LongMid = 40;

int powerToLaunch_Mid = 65;
int positionToStop_Mid = 22;

int powerToLaunch_Short = 37;
int positionToStop_Short = 23;

//int powerToLaunch_Mid = 80;
//int positionToStop_Mid = 1440;
//int positionToStop_Mid = postionToDown + 50;

long lastLaunchTime = 0;


//int postionToDown = 1280;
//int postionToDown = 2900;

// Position to break launcher

void TravelToTheOtherSide();
//void ForBack(int originalPower, int distance);
void StopMoving();
void StopOrReverseBelt();
int AdjustPowerUsingBatteryLevel(int originalPower);
int AdjustPowerUsingExternalBatteryLevel(int originalPower);
void LauncherUp_Helper();
void LaunchBall_Helper();
void LauncherDown_Helper();
void RotateHelper(int power);
void LaunchBall_Autonomous();
void PickBalls();
void SonarRotate(int power, int distance);
void SonarForBack(int power, int distance);
void EncoderRotate(int power, int distance);
void EncoderForBack(int power, int distance);

void ForBackHelper(int power);

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	SensorValue[Led1] = false;

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

	if (SensorValue[Jumper1] == 0)
	{
		startTask(LaunchBall_ProgrammingSkill);
	}
	else
	{
		//startTask(StopAutonomous);
		LaunchBall_Autonomous();
		PickBalls();
	}

}

//task StopAutonomous
//{
//	while(nPgmTime < 15000)
//	{}
//	stopTask(autonomous);
//}

void PickBalls()
{
	nMotorEncoder(FrontLeft) = 0;
	EncoderRotate(40, 97);

	wait1Msec(100);
	
	startTask(StartBelt);

	nMotorEncoder(FrontLeft) = 0;
	EncoderForBack(30, 1000);

	//wait1Msec(1500);
	
	//nMotorEncoder(FrontLeft) = 0;
	//EncoderRotate(-40, 140);
	
	//LauncherRange = LongMid;
	//startTask(AutoLaunchBall);

	
	//SonarForBack();


	//EncoderRotate(-40, 80);

	////ForBackHelper(50);
	//nMotorEncoder(FrontLeft) = 0;
	//EncoderForBack(70, 400);
	//wait1Msec(100);
	//SonarRotate(38, 400);
	////nMotorEncoder(FrontLeft) = 0;
	////EncoderRotate(40, 90);
	//wait1Msec(100);
	//startTask(StartBelt);

	//nMotorEncoder(FrontLeft) = 0;
	//EncoderForBack(30, 650);	
	
}

void PickBalls_Old()
{
	//SonarForBack();


	//EncoderRotate(-40, 80);

	//ForBackHelper(50);
	nMotorEncoder(FrontLeft) = 0;
	EncoderForBack(70, 320);
	wait1Msec(100);
	//SonarRotate(38, 350);
	nMotorEncoder(FrontLeft) = 0;
	EncoderRotate(40, 90);
	wait1Msec(100);
	startTask(StartBelt);

	nMotorEncoder(FrontLeft) = 0;
	EncoderForBack(30, 650);	
	
	//wait1Msec(2000);
	
	//nMotorEncoder(FrontLeft) = 0;
	//EncoderRotate(-70, 160);

	//wait1Msec(2000);

	//LauncherRange = LongMid;
	//startTask(AutoLaunchBall);
			
	//SonarForBack(50, 150);
}

void SonarRotate(int power, int distance)
{
	RotateHelper(power);
	wait1Msec(1000);

	while (SensorValue[SonarSensor] == -1 || abs(SensorValue[SonarSensor]) > distance  /*&& abs(nMotorEncoder(FrontLeft)) < 450 */)
	{
		RotateHelper(power);
	}

	StopMoving();
}

void RotateHelper(int power)
{
	int adjustedPower = AdjustPowerUsingBatteryLevel(power);

	motor[FrontLeft] = adjustedPower;
	motor[FrontRight] = -adjustedPower;
	motor[BackLeft] = adjustedPower;
	motor[BackRight] = -adjustedPower;
}

void SonarForBack(int power, int distance)
{
	ForBackHelper(power);

	//while (abs(SensorValue[SonarSensor]) > 100 /*&& abs(nMotorEncoder(FrontLeft)) < 450 */)
	//{
	//	ForBackHelper(power);
	//}

	while (abs(SensorValue[SonarSensor]) < distance /*&& abs(nMotorEncoder(FrontLeft)) < 450 */)
	{
		ForBackHelper(power);
	}

	StopMoving();
}

void EncoderForBack(int power, int distance)
{
	//if (power <= 0) return;

	int current = abs(nMotorEncoder(FrontLeft));
	float offset = 0;

	// RED only for programming skill
	// power = -power;
	while (current + offset < distance)
	{
		int previous = current;

		ForBackHelper(power);
		//motor[FrontLeft] = power;
		//motor[FrontRight] = power;
		//motor[BackRight] = power;
		//motor[BackLeft] = power;

		int current = abs(nMotorEncoder(FrontLeft));

		offset = current - previous;
		offset = offset * 1.6;
	}

	StopMoving();
}



void ForBackHelper(int power)
{
	int adjustedPower = AdjustPowerUsingBatteryLevel(power);

	motor[FrontLeft] = -adjustedPower;
	motor[FrontRight] = adjustedPower;
	motor[BackLeft] = adjustedPower;
	motor[BackRight] = -adjustedPower;
}

task StartBelt()
{
	int beltPower = 125;
	beltPower = AdjustPowerUsingBatteryLevel(beltPower);
	motor[Belt] = beltPower;
}

void LaunchBall_Autonomous()
{
	writeDebugStreamLine("(autonomous started) Time: %d, %d", nPgmTime, nSysTime);

	int i = 0;
	int j = 0;
	while(true)
	{
		// Repeat for 4 times (4 preloads)
		if (i > 3 || j > 5)
		//if (i > 0 || j > 5)
		{
			break;
		}

		// move backward to aligh balls
		motor[Belt] = -70;
		wait1Msec(150);
		motor[Belt] = 0;


		//int launcherPosition = SensorValue[LauncherPosition];
		//writeDebugStreamLine("LaunchBall) launcherPosition :%d", launcherPosition);

		// Down the launcher
		int originalPower = powerToDown; // power to down the launcher

		if (i <= 3)
		{
			//originalPower += 1;
		}

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
		//clearTimer(T3);
		//while(SensorValue[LauncherPosition] > postionToDown && time1[T3] < 1000)
		//{
		//}

		wait1Msec(globalWaiter);

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
		if (i == 0)
		{
			startTask(MoveBeltToReadyFirstBall);
		}
		else
		{
			startTask(MoveBeltToReadyBall);
		}
		// Wait until a ball loaded
		clearTimer(T3);
		while(SensorValue[BallLoaded] == 0 && time1[T3] < 3000)
		{
		}

		// Close the dispenser
		startTask(CloseDispenser);

		j++;

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
			wait1Msec(400);
			startTask(LauncherUp);

			//wait1Msec(150);
			//startTask(LauncherStop);

			// Increase
			i++;
		}
		// Delay for 1 sec before the nexr launch
		wait1Msec(700);
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

task LauncherUp()
{
	LauncherUp_Helper();
}

void LauncherUp_Helper()
{ // Timer
	startTask(LauncherStop);

	int originalPower = powerToLaunch;
	int originalPower_external = powerToLaunch;

	int local_positionToStop = positionToStop;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(time1[T1] < 110)
	{
	}
	//writeDebugStreamLine("LauncherUp) launcherPosition #1: %d", abs(nMotorEncoder(Launcher4)));

	//while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 1000)
	//{
	//}

	writeDebugStreamLine("LauncherUp) launcherPosition #1: %d", abs(nMotorEncoder(Launcher4)));

	//int extPower = 20;
	//motor[Launcher1] = extPower;
	//motor[Launcher2] = extPower;
	//motor[Launcher3] = extPower;
	//motor[Launcher4] = extPower;
	//wait1Msec(70);

	startTask(LauncherStop);
	writeDebugStreamLine("LauncherUp) launcherPosition #2: %d", abs(nMotorEncoder(Launcher4)));

}

void LauncherUp_Helper_IEM()
{
	startTask(LauncherStop);

	int originalPower = powerToLaunch;
	int originalPower_external = powerToLaunch;

	int local_positionToStop = positionToStop;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	//while(time1[T1] < 120)
	//{
	//}
	//writeDebugStreamLine("LauncherUp) launcherPosition #1: %d", abs(nMotorEncoder(Launcher4)));

	while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 1000)
	{
	}

	writeDebugStreamLine("LauncherUp) launcherPosition #1: %d", abs(nMotorEncoder(Launcher4)));

	//int extPower = 20;
	//motor[Launcher1] = extPower;
	//motor[Launcher2] = extPower;
	//motor[Launcher3] = extPower;
	//motor[Launcher4] = extPower;
	//wait1Msec(70);

	startTask(LauncherStop);
	writeDebugStreamLine("LauncherUp) launcherPosition #2: %d", abs(nMotorEncoder(Launcher4)));

}

task LauncherUp_Longest()
{
	startTask(LauncherStop);

	//int positionToStop = 1480;

	int originalPower = powerToLaunch_Longest;
	int originalPower_external = powerToLaunch_Longest;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	clearTimer(T1);
	while(time1[T1] < 130)
		//while(SensorValue[LauncherPosition] < positionToStop_Longest && time1[T1] < 500)
	{
	}

	//if (time1[T1] > 500)
	//{
	//	startTask(LauncherStop);
	//	return;
	//}

	startTask(LauncherStop);
	//int power1 = 0;
	//motor[Launcher1] = power1;
	//motor[Launcher2] = power1;
	//motor[Launcher3] = power1;
	//motor[Launcher4] = power1;
}

task LauncherUp_LongMid()
{
	startTask(LauncherStop);

	int originalPower = powerToLaunch_LongMid;
	int originalPower_external = powerToLaunch_LongMid;

	int local_positionToStop = positionToStop_LongMid;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_LongMid) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 1000)
	{
	}

	//int extPower = 20;
	//motor[Launcher1] = extPower;
	//motor[Launcher2] = extPower;
	//motor[Launcher3] = extPower;
	//motor[Launcher4] = extPower;
	//wait1Msec(70);

	startTask(LauncherStop);
}

task LauncherUp_Mid()
{
	startTask(LauncherStop);

	int originalPower = powerToLaunch_Mid;
	int originalPower_external = powerToLaunch_Mid;

	int local_positionToStop = positionToStop_Mid;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Mid) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 500)
	{
	}

	int extPower = 20;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(70);

	startTask(LauncherStop);
}

task LauncherUp_Short()
{
	startTask(LauncherStop);

	int originalPower = powerToLaunch_Short;
	int originalPower_external = powerToLaunch_Short;

	int local_positionToStop = positionToStop_Short;

	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower) * -1;
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower_external) * -1;
	writeDebugStreamLine("LauncherUp_Short) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	//int local_positionToStop = 20;
	//wait1Msec(35);
	//unsigned long time = nPgmTime + 85;
	//while(nPgmTime < time)
	//while(SensorValue[LauncherPosition] < local_positionToStop && time1[T1] < 500)
	//for(int i=0;i<3500;i++)
	while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 500)
	{
	}

	int extPower = 20;
	motor[Launcher1] = extPower;
	motor[Launcher2] = extPower;
	motor[Launcher3] = extPower;
	motor[Launcher4] = extPower;
	wait1Msec(70);

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
{
	LauncherDown_Helper();

	// T3
	//int power = 70; // perfect but strong


	//clearTimer(T3);
	//while(SensorValue[LauncherPosition] > postionToDown && time1[T3] < 1000)
	//{
	//}

	//clearTimer(T3);
	//while(time1[T3] < 3000)
	//{
	//}

	//startTask(LauncherStop);
	/*
	clearTimer(T3);
	while(SensorValue[BallLoaded] == 0 && time1[T3] < 2000)
	{
	}
	//if(time1[T3] >= 2000)
	//{
	//	startTask(LauncherStop);
	//}
	//else
	//{
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
	else if (LauncherRange == Longest)
	{
	startTask(LauncherUp_Longest);
	}
	else
	{
	startTask(LauncherUp);
	}
	//}
	*/
}

void LauncherDown_Helper_Touch()
{
	//int power = motor[Launcher1];

	//if (power != 0)
	//{
	//	return;
	//}

	int originalPower = powerToDown;
	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("LauncherDown_Helper) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);


	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	while(SensorValue[LauncherReady] == 0 && time1[T1] < 80)
	{
	}

	writeDebugStreamLine("LauncherDown_Helper #1) LauncherPosition: %d", abs(nMotorEncoder(Launcher4)));


	clearTimer(T1);

	//originalPower = powerToDown2;
	//primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	//externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	//motor[Launcher1] = externalPower;
	//motor[Launcher2] = externalPower;
	//motor[Launcher3] = primaryPower;
	//motor[Launcher4] = primaryPower;

	int local_positionToStop = postionToDown;

	while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 1000)
	{
	}

	originalPower = powerToStay;
	primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	writeDebugStreamLine("LauncherDown_Helper #2) LauncherPosition: %d", abs(nMotorEncoder(Launcher4)));
}


void LauncherDown_Helper_IEM()
{
	int power = motor[Launcher1];

	if (power != 0)
	{
		return;
	}

	int local_positionToStop = postionToDown;
	int originalPower = powerToDown;
	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("LauncherDown_Helper) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);


	nMotorEncoder(Launcher4) = 0;
	clearTimer(T1);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	while(abs(nMotorEncoder(Launcher4)) < local_positionToStop && time1[T1] < 1000)
	{
	}

	writeDebugStreamLine("LauncherDown_Helper #1) LauncherPosition: %d", abs(nMotorEncoder(Launcher4)));

	originalPower = powerToStay;
	primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	writeDebugStreamLine("LauncherDown_Helper #2) LauncherPosition: %d", abs(nMotorEncoder(Launcher4)));

}

void LauncherDown_Helper()
{
	int originalPower = powerToDown;
	int primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	int externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("LauncherDown_Helper) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

	wait1Msec(globalWaiter);

	originalPower = powerToStay;
	primaryPower = AdjustPowerUsingBatteryLevel(originalPower);
	externalPower = AdjustPowerUsingExternalBatteryLevel(originalPower);
	writeDebugStreamLine("LauncherDown_Helper) primaryPower:%d,  externalPower: %d", primaryPower, externalPower);

	motor[Launcher1] = externalPower;
	motor[Launcher2] = externalPower;
	motor[Launcher3] = primaryPower;
	motor[Launcher4] = primaryPower;

}

task LaunchBall()
{
	for (int i=0;i<10;i++)
		//while(true)
	{
		LaunchBall_Helper();
	}
}

void LaunchBall_Helper()
{
	SensorValue[Led1] = false;

	LauncherDown_Helper();
	//startTask(LauncherDown);

	SensorValue[Led1] = true;

	clearTimer(T3);
	while(SensorValue[BallLoaded] == 0 && time1[T3] < 4000)
	{
	}

	SensorValue[Led1] = false;

	stopTask(LauncherDown);

	if (SensorValue[BallLoaded] == 1)
	{
		//wait1Msec(450);
		wait1Msec(300);
		startTask(LauncherUp);

		//if (LauncherRange == Near)
		//{
		//	startTask(LauncherUp_Short);
		//}
		//else if (LauncherRange == Middle)
		//{
		//	startTask(LauncherUp_Mid);
		//}
		//else if (LauncherRange == LongMid)
		//{
		//	startTask(LauncherUp_LongMid);
		//}
		//else if (LauncherRange == Longest)
		//{
		//	startTask(LauncherUp_Longest);
		//}
		//else
		//{
		//	startTask(LauncherUp);
		//}
		//wait1Msec(200);
		//startTask(LauncherStop);
		wait1Msec(300);
	}
	else
	{
		startTask(LauncherStop);
	}
}

task LaunchBall_ProgrammingSkill()
{ // Skills
	int i = 0;
	while(true)
	{
		stopTask(LauncherUp);
		startTask(LauncherStop);

		//int launcherPosition = SensorValue[LauncherPosition];
		//writeDebugStreamLine("LaunchBall) launcherPosition :%d", launcherPosition);

		LauncherDown_Helper();
		//clearTimer(T3);
		//while(SensorValue[LauncherPosition] > postionToDown /* - 10*/ && time1[T3] < 1000)
		//{
		//}

		clearTimer(T3);
		while(SensorValue[BallLoaded] == 0 && time1[T3] < 1000)
		{
		}
		i++;
		if(time1[T3] >= 1000)
		{
			//startTask(LauncherUp);
			startTask(LauncherStop);

		}
		else
		{
			wait1Msec(330); // Skill
			//wait1Msec(500); // Drive
			if (LauncherRange == Longest)
			{
				startTask(LauncherUp_Longest);
			}
			else
			{
				startTask(LauncherUp);
			}
			//startTask(LauncherUp_Skill);

			//wait1Msec(150);
			//startTask(LauncherStop);
		}
		//if (i == 32)
		//{
		//	TravelToTheOtherSide();
		//	wait1Msec(300); // Skill
		//}

		wait1Msec(300); // Skill

		//wait1Msec(300); // Drive
	}
}

void ForBack_Old(int originalPower, int distance)
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
	int current = abs(nMotorEncoder(FrontLeft));
	float offset = 0;

	if (power == 0) return;

	//power = -power;
	while (current + offset < distance)
	{
		int previous = current;
		
		RotateHelper(power);

		int current = abs(nMotorEncoder(FrontLeft));

		offset = current - previous;
		offset = offset * 1.6;
	}

	StopMoving();
}

void TravelToTheOtherSide()
{
	wait1Msec(100);

	// Turn right to the center
	nMotorEncoder(FrontRight) = 0;
	EncoderRotate(-50, 38);

	wait1Msec(100);

	// Move forward
	nMotorEncoder(FrontRight) = 0;
	EncoderForBack(-80, 920);

	wait1Msec(200);

	// Turn left to the other side
	nMotorEncoder(FrontRight) = 0;
	EncoderRotate(50, 420);

	wait1Msec(100);

	// Move backward
	nMotorEncoder(FrontRight) = 0;
	EncoderForBack(80, 640);

	wait1Msec(100);

	// Turn right to the goal
	//nMotorEncoder(FrontRight) = 0;
	//EncoderRotate(-50, 25);

	//wait1Msec(100);
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
	LauncherRange = Long;

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
		motor[FrontLeft] = power3 - power1;
		motor[FrontRight] = -power3 + power1;//* 90 / 100;
		motor[BackRight] = power3 + power1;// * 90 / 100;
		motor[BackLeft] = -power3 - power1;// * 90 / 100;


		if (btn8u == 1)
		{
			//nMotorEncoder(FrontRight) = 0;
			//ForBack(-80, 1000);
			if (motor[Launcher1] == 0)
			{
			}
			else
			{

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
				else if (LauncherRange == Longest)
				{
					startTask(LauncherUp_Longest);
				}
				else
				{
					startTask(LauncherUp);
				}
			}
		}
		else if (btn8d == 1)
		{
			//stopTask(AutoLaunchBall);
			//wait1Msec(100);

			//LauncherRange = Longest;
			//startTask(AutoLaunchBall);

			//wait1Msec(100);

			LauncherRange = Near;

			stopTask(LauncherUp);
			startTask(LauncherDown);

		}
		else if (btn8l == 1)
		{

			startTask(LaunchBall);
		}
		else if (btn8r == 1)
		{
			if (SensorValue[Jumper1] == 0)
			{
				TravelToTheOtherSide();
			}
			else
			{
				//nMotorEncoder(FrontRight) = 0;
				//EncoderRotate(50, 250);

				// Open or Close the ball dispenser
				if (SensorValue[BallDispenserPosition] >= 1300)
				{
					// Open
					//stopTask(CloseDispenser);
					//startTask(OpenDispenser);
					motor[BallDispenser] = -powerForDispenser;

					clearTimer(T2);
					while(SensorValue[BallDispenserPosition] > 1070 && time1[T2] < 5000)
					{
					}

					motor[BallDispenser] = 0;
				}
				else
				{	// Close
					//stopTask(OpenDispenser);
					//startTask(CloseDispenser);

					motor[BallDispenser] = powerForDispenser;

					clearTimer(T2);
					while(SensorValue[BallDispenserPosition] < 1460 && time1[T2] < 5000)
					{
					}

					motor[BallDispenser] = 0;

				}
			}
		}
		else if (btn6u == 1)
		{
			int beltPower = 110;
			beltPower = AdjustPowerUsingBatteryLevel(beltPower);

			motor[Belt] = beltPower;
			//motor[Belt] = 80;
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
		}
		else if (btn7u == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = Middle;
			startTask(AutoLaunchBall);
		}
		else if (btn7l == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = LongMid;
			startTask(AutoLaunchBall);
		}

		else if (btn7d == 1)
		{
			stopTask(AutoLaunchBall);
			wait1Msec(100);

			LauncherRange = Long;
			startTask(AutoLaunchBall);
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

/*
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
*/
task OpenDispenser()
{// T2
	motor[BallDispenser] = -powerForDispenser;

	clearTimer(T2);
	while(SensorValue[BallDispenserPosition] > 1070 && time1[T2] < 5000)
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

	motor[BallDispenser] = powerForDispenser;

	clearTimer(T2);
	while(SensorValue[BallDispenserPosition] < 1460 && time1[T2] < 5000)
	{
	}

	motor[BallDispenser] = 0;
}

task AutoLaunchBall()
{
	//int power = motor[Launcher1];

	//if (power != 0)
	//{
	//	return;
	//}

	// T2
	startTask(CloseDispenser);

	int beltPower = 110;
	beltPower = AdjustPowerUsingBatteryLevel(beltPower);
	motor[Belt] = -beltPower;

	//motor[Belt] = -110;
	wait1Msec(150);
	//wait1Msec(200);
	motor[Belt] = 0;

	//int launcherPosition = SensorValue[LauncherPosition];
	//writeDebugStreamLine("LaunchBall) launcherPosition :%d", launcherPosition);


	//if ((nPgmTime - lastLaunchTime) > 5000)
	//{
	//	originalPower += 4;
	//}

	LauncherDown_Helper();

	//clearTimer(T3);
	//while(SensorValue[LauncherPosition] > postionToDown && time1[T3] < 1000)
	//{
	//}

	startTask(OpenDispenser);

	if ((nPgmTime - lastLaunchTime) > 5000)
	{
		startTask(MoveBeltToReadyFirstBall);
	}
	else
	{
		startTask(MoveBeltToReadyBall);
	}

	clearTimer(T3);
	//bool ballReleased = false;

	int ballReleased = 0;
	int ballLoaded = 0;

	while(ballReleased == 0 && ballLoaded == 0 && time1[T3] < 4000)
	{
		ballReleased = SensorValue[BallReleased];
		ballLoaded = SensorValue[BallLoaded];
	}


	if (ballReleased > 0)
	{

		wait1Msec(150);

		//ballReleased = true;
	}


	//if (SensorValue[BallLoaded] > 0)
	//{
	//	BallLoaded = true;
	//}

	startTask(CloseDispenser);

	lastLaunchTime = nPgmTime;

	if(time1[T3] >= 4000)
	{
		startTask(LauncherStop);
	}
	else if (ballReleased == 1)
	{
		ballLoaded = 0;
		while(ballLoaded == 0 && time1[T3] < 4000)
		{
			ballLoaded = SensorValue[BallLoaded];
		}
	}

	if (SensorValue[BallLoaded] == 1 || ballLoaded == 1)
	{
		//wait1Msec(450);
		wait1Msec(400);

		if (LauncherRange == Near)
		{
			startTask(LauncherUp_Short);
		}
		else if (LauncherRange == Middle)
		{
			startTask(LauncherUp_Mid);
		}
		else if (LauncherRange == LongMid)
		{
			startTask(LauncherUp_LongMid);
		}
		else if (LauncherRange == Longest)
		{
			startTask(LauncherUp_Longest);
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
	//motor[Belt] = 60;

	int beltPower = 110;
	beltPower = AdjustPowerUsingBatteryLevel(beltPower);

	motor[Belt] = beltPower;
	//motor[Belt

	clearTimer(T4);
	while(time1[T4] < 1300)
	{
	}

	motor[Belt] = 0;

}

task MoveBeltToReadyFirstBall()
{// T4

	// move backward to aligh balls


	// move backward to aligh balls
	int beltPower = 110;
	beltPower = AdjustPowerUsingBatteryLevel(beltPower);
	motor[Belt] = -beltPower;

	wait1Msec(200);
	motor[Belt] = 0;

	beltPower = 110;
	beltPower = AdjustPowerUsingBatteryLevel(beltPower);

	motor[Belt] = beltPower;

	clearTimer(T4);
	while(time1[T4] < 1300)
	{
	}

	motor[Belt] = 0;

}
