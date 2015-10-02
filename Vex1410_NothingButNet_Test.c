#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    ExternalBatteryValue, sensorNone)
#pragma config(Sensor, dgtl1,  touchSensorLaunched, sensorNone)
#pragma config(Sensor, dgtl2,  touchSensorLoaded, sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           MidBelt,       tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           LauncherUp1,   tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           LauncherUp2,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           LauncherDown1, tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port5,           LauncherDown2, tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           BottomBelt,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           TopBelt,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           WheelRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           WheelLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          MiddleWheel,   tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
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

// tasks
// task warmUpLauncher();
task startLauncher();
task stopLauncher();
task startBelt();
task startMidBelt();
task stopBelt();
task startShortGameMode();
task startGameMode();
task stopGameMode();
task launchBall();
task launchBall_UserControl();
task displayBatteryLevelOnLCD();

/// global variables
// Lancher Range Mode
int LauncherRange;

// Lancher Range Mode
int BeltSpeed;

float LauncherPowerOffset;

//bool IsLauncherRunning;

// global subroutines
void WarmUpLauncher();
//void MakeLauncherIdle();
int GetPowerFlywheelUp();
int GetPowerFlywheelDown();
int GetBeltPower();
int GetMidBeltPower();
void AdjustSkill(float up, float down);
int AdjustPowerUsingBatteryLevel(int originalPower);
int AdjustPowerUsingExternalBatteryLevel(int originalPower);
//void StartAndControlLauncher();
int GetEncoderLauncherUp();
int GetEncoderLauncherDown();
//float AdjustLauncherPower();
void ClearEncoder();
//void ClearEncoderUp();
//void ClearEncoderDown();
//int GetExpectedSpeedFlywheelUp();
//int GetExpectedSpeedFlywheelDown();
void StopOrReverseBelt();
void CheckAndAdjust();

// properties
enum GameMode{
	Far = 0,
	Middle = 1,
	Near = 2,

	Fast = 10,
	Slow = 11,

	AutonomousMode = 20,
	AutonomousModeShort = 21,

	Skill = 22,
	UserControlMode = 23,

	WarmUP = 30,
	WarmUP2 = 31,

	Test = 99,
	MidBeltPower = 100,
};

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	//bLCDBacklight=true;

	//startTask(displayBatteryLevelOnLCD);


	//bLCDBacklight=false;

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
		wait1Msec(5000);
	//}

		bLCDBacklight=false;

}
task autonomous_old()
{
	//startTask(displayBatteryLevelOnLCD);

	WarmUpLauncher();
	//wait1Msec(1000);

	LauncherRange = AutonomousMode;
	BeltSpeed = AutonomousMode;

	startTask(startLauncher);
	wait1Msec(1100);

	startTask(startBelt);
	startTask(startMidBelt);

	//wait1Msec(13000);

	//wait1Msec(120000);

	//StartTask(stopLauncher);
	//StartTask(stopBelt);
}

task autonomous()
{
	//startTask(displayBatteryLevelOnLCD);

	WarmUpLauncher();
	//wait1Msec(1000);

	LauncherRange = UserControlMode;
	BeltSpeed = UserControlMode;

	startTask(startLauncher);

	wait1Msec(1100);

	if (nImmediateBatteryLevel < 7900)
	{
			wait1Msec(200);
	}

	startTask(startBelt);
	startTask(startMidBelt);

	int index = 0;

	while(true)
	{
		startTask(startLauncher);

		if (index <= 1)
		{
			float powerUp = motor[LauncherUp1];
			float powerDown = motor[LauncherDown1];

			float powerUpNew = powerUp * 1.0;
			float powerDownNew = powerDown * 1.0;

			if (false)
			{ // low battery
				powerUpNew = powerUp * 0.9;
				powerDownNew = powerDown * 0.9;
			}

			motor[LauncherUp1] = powerUpNew;
			motor[LauncherUp2] = powerUpNew;
			motor[LauncherDown1] = powerDownNew;
			motor[LauncherDown2] = powerDownNew;
		}
		/*if (time1[T3] > 2000 && index > 0)
		{
		MakeLauncherIdle();
		clearTimer(T3);

		}*/

		if (SensorValue[touchSensorLoaded] == 1 && time1[T3] > 1300)
		{
			float powerUp = motor[LauncherUp1];
			float powerDown = motor[LauncherDown1];

			writeDebugStreamLine("index:%d, T3: %d, PowerUp: %d, PowerDown: %d", ++index, time1[T3], powerUp, powerDown);
			clearTimer(T3);
		}

	}
	//wait1Msec(13000);

	//wait1Msec(13000);

	//wait1Msec(120000);

	//startTask(stopLauncher);
	//startTask(stopBelt);
}

task launchBall_UserControl()
{
	clearTimer(T3);

	LauncherRange = UserControlMode;
	BeltSpeed = UserControlMode;

	startTask(startBelt);
	startTask(startMidBelt);

	WarmUpLauncher();

	LauncherRange = UserControlMode;
	BeltSpeed = UserControlMode;

	startTask(startBelt);
	startTask(startMidBelt);

	//wait1Msec(800);

	//wait1Msec(1000);

	int index = 0;


	while(true)
	{
		startTask(startLauncher);

		if (index <= 0)
		{
			float powerUp = motor[LauncherUp1];
			float powerDown = motor[LauncherDown1];

			float powerUpNew = powerUp * 1.0;
			float powerDownNew = powerDown * 1.0;

			if (true)
			{ // low battery
				powerUpNew = powerUp * 0.80;
				//powerDownNew = powerDown * 0.9;
			}

			motor[LauncherUp1] = powerUpNew;
			motor[LauncherUp2] = powerUpNew;
			motor[LauncherDown1] = powerDownNew;
			motor[LauncherDown2] = powerDownNew;
		}
		/*if (time1[T3] > 2000 && index > 0)
		{
		MakeLauncherIdle();
		clearTimer(T3);

		}*/

		if (SensorValue[touchSensorLoaded] == 1 && time1[T3] > 1300)
		{
			float powerUp = motor[LauncherUp1];
			float powerDown = motor[LauncherDown1];

			writeDebugStreamLine("index:%d, T3: %d, PowerUp: %d, PowerDown: %d", ++index, time1[T3], powerUp, powerDown);
			clearTimer(T3);
		}

	}
	//wait1Msec(13000);

	//wait1Msec(120000);

	startTask(stopLauncher);
	startTask(stopBelt);
}

/*
void MakeLauncherIdle()
{
int powerMidBelt = motor[MidBelt];
int powerTopBelt = motor[TopBelt];
motor[MidBelt] = 0;
motor[TopBelt] = motor[BottomBelt] = 0;

stopTask(startMidBelt);

float powerUp = motor[LauncherUp1];
float powerDown = motor[LauncherDown1];

float powerUpNew = powerUp * 0.8;
float powerDownNew = powerDown * 0.8;

motor[LauncherUp1] = powerUpNew;
motor[LauncherUp2] = powerUpNew;
motor[LauncherDown1] = powerDownNew;
motor[LauncherDown2] = powerDownNew;

wait1Msec(500);

motor[LauncherUp1] = powerUp;
motor[LauncherUp2] = powerUp;
motor[LauncherDown1] = powerDown;
motor[LauncherDown2] = powerDown;

//wait1Msec(1000);

//resumeTask(startMidBelt);

motor[MidBelt] = powerMidBelt;
motor[TopBelt] = motor[BottomBelt] = powerTopBelt;

}
*/
task launchBall()
{
	LauncherRange = Skill;
	BeltSpeed = Skill;

	startTask(startLauncher);
	wait1Msec(1000);

	startTask(startBelt);
	startTask(startMidBelt);

	float launcherUp;
	float launcherDown;

	clearTimer(T1);
	ClearEncoder();
	clearTimer(T2);
	//clearTimer(T3);

	bool loaded = false;
	int index = 0;
	while(true)
	{
		//if (time1[T3] >= 200)
		//{
		//	launcherUp = GetEncoderLauncherUp();
		//	launcherDown = GetEncoderLauncherDown();
		//	writeDebugStreamLine("Up: %d, Down: %d", launcherUp, launcherDown);

		//	clearTimer(T1);
		//	ClearEncoder();
		//}

		if (time1[T1] >= 1500 && SensorValue[touchSensorLoaded] == 1)
		{
			loaded = true;
			//wait1Msec(30);

			//writeDebugStreamLine(">>>>>>>>> Launched");

			//startTask(startLauncher);
			//MakeLauncherIdle();
			//clearTimer(T1);
			//clearTimer(T2);

			clearTimer(T3);
			startTask(startLauncher);

			int powerMidBelt = motor[MidBelt];
			int powerTopBelt = motor[TopBelt];

			motor[MidBelt] = 0;
			motor[TopBelt] = motor[BottomBelt] = 0;

			ClearEncoder();
			wait1Msec(200);
			launcherUp = GetEncoderLauncherUp();
			launcherDown = GetEncoderLauncherDown();

			AdjustSkill(launcherUp, launcherDown);


			writeDebugStreamLine(">>>>>>>>> Launched: T3:%d, index: %d,  Up: %d, Down: %d", time1[T3], index++, launcherUp, launcherDown);

			motor[MidBelt] = powerMidBelt;
			motor[TopBelt] = motor[BottomBelt] = powerTopBelt;

			clearTimer(T1);
			clearTimer(T2);
		}
		/*
		if (loaded == true && SensorValue[touchSensorLaunched] == 1)
		{
		}
		*/
		/*
		if (time1[T2] >= 3000)
		//if (false)
		{
		startTask(startLauncher);
		//writeDebugStreamLine("<<<<<<<<< Idle Launcher Started");
		MakeLauncherIdle();
		writeDebugStreamLine("<<<<<<<<< Idle Launcher Done");

		clearTimer(T2);
		}*/

	}
}

task stopGameMode()
{
	stopTask(launchBall);
	stopTask(launchBall_UserControl);

	stopTask(autonomous);
	stopTask(startGameMode);
	stopTask(startShortGameMode);

	startTask(stopLauncher);
	startTask(stopBelt);
}


task startShortGameMode()
{

	LauncherRange = WarmUP;
	startTask(startLauncher);
	wait1Msec(1000);

	LauncherRange = Near;
	BeltSpeed = Slow;
	LauncherPowerOffset = 0;

	startTask(startLauncher);

	//wait1Msec(2000);

	//startTask(startBelt);
}

task startGameMode()
{
	WarmUpLauncher();

	//LauncherRange = Far;
	//LauncherRange = UserControlMode;
	//BeltSpeed = Slow;
	LauncherPowerOffset = 0;

	startTask(startLauncher);

	//float launcherUp;
	//float launcherDown;

	clearTimer(T1);
	ClearEncoder();
	clearTimer(T2);
	//clearTimer(T3);

	//int index = 0;
	while(true)
	{
		if (time1[T1] >= 1500 && SensorValue[touchSensorLoaded] == 1)
		{
			clearTimer(T3);
			startTask(startLauncher);

			int powerMidBelt = motor[MidBelt];
			int powerTopBelt = motor[TopBelt];

			motor[MidBelt] = 0;
			motor[TopBelt] = motor[BottomBelt] = 0;

			CheckAndAdjust();

			motor[MidBelt] = powerMidBelt;
			motor[TopBelt] = motor[BottomBelt] = powerTopBelt;

			clearTimer(T1);
			clearTimer(T2);
		}
		/*
		if (time1[T2] >= 3000)
		//if (false)
		{
		startTask(startLauncher);


		//writeDebugStreamLine("<<<<<<<<< Idle Launcher Started");
		MakeLauncherIdle();
		writeDebugStreamLine("<<<<<<<<< Idle Launcher Done");

		clearTimer(T2);
		}*/

	}
}

void CheckAndAdjust()
{
	writeDebugStreamLine(">>>>>>>>>>>>>> CheckAndAdjust");

	ClearEncoder();
	wait1Msec(200);
	float launcherUp = GetEncoderLauncherUp();
	float launcherDown = GetEncoderLauncherDown();
	AdjustSkill(launcherUp, launcherDown);


	writeDebugStreamLine(">>>>>>>>> Launched: T3:%d, Up: %d, Down: %d", time1[T3], launcherUp, launcherDown);
	//writeDebugStreamLine(">>>>>>>>> Launched: T3:%d, index: %d,  Up: %d, Down: %d", time1[T3], index++, launcherUp, launcherDown);

}


void AdjustSkill(float up, float down)
{
	float powerUp = motor[LauncherUp1];
	float powerDown = motor[LauncherDown1];

	bool adjusted = false;
	//if (up < 95 || 99 < up)
	if (up < 95)
	{
		writeDebugStreamLine("---------- Adjusted up < 95");

		adjusted = true;
		float powerUpNew = powerUp * 1.2;
		motor[LauncherUp1] = powerUpNew;
		motor[LauncherUp2] = powerUpNew;
	}
	else if (99 < up)
	{
		writeDebugStreamLine("---------- Adjusted 99 < up");

		adjusted = true;
		float powerUpNew = powerUp * 0.8;
		motor[LauncherUp1] = powerUpNew;
		motor[LauncherUp2] = powerUpNew;
	}

	if (down < 105)
	{
		writeDebugStreamLine("---------- Adjusted down < 105");

		adjusted = true;
		float powerDownNew = powerDown * 1.2;
		motor[LauncherDown1] = powerDownNew;
		motor[LauncherDown2] = powerDownNew;
	}
	else if (110 < down)
	{
		writeDebugStreamLine("---------- Adjusted 110 < down");

		adjusted = true;
		float powerDownNew = powerDown * 0.8;
		motor[LauncherDown1] = powerDownNew;
		motor[LauncherDown2] = powerDownNew;
	}

	if (adjusted == true)
	{
		wait1Msec(500);

		motor[LauncherUp1] = powerUp;
		motor[LauncherUp2] = powerUp;
		motor[LauncherDown1] = powerDown;
		motor[LauncherDown2] = powerDown;
	}
	else
	{
		writeDebugStreamLine("---------- Adjust Skill skipped -----------");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task startLauncher()
{
	// up:100, down:40 = mid range
	// up:40, down:100 = mid range
	// up:0, down:100 = mid-short range
	// up:80, down:0 = short range

	//ClearEncoder();

	int power_Up = GetPowerFlywheelUp();
	int power_Down = GetPowerFlywheelDown();
	//	int power_Up = 50;
	//	int power_Down = 50;

	//writeDebugStreamLine("power_Up: %d, power_Down: %d", power_Up, power_Down);

	motor[LauncherUp1] = power_Up;
	motor[LauncherUp2] = power_Up;
	motor[LauncherDown1] = power_Down;
	motor[LauncherDown2] = power_Down;
}

task stopLauncher()
{
	motor[LauncherUp1] = 0;
	motor[LauncherUp2] = 0;
	motor[LauncherDown1] = 0;
	motor[LauncherDown2] = 0;
}


task startBelt()
{
	int power = GetBeltPower();
	motor[TopBelt] = motor[BottomBelt] = power;
	//	motor[Belt] = 30;
}

task startMidBelt()
{
	motor[MidBelt] = GetMidBeltPower();
	//	motor[Belt] = 30;
}

task stopBelt()
{
	motor[TopBelt] = motor[BottomBelt] = 0;
	motor[MidBelt] = 0;
}
/*
int GetExpectedSpeedFlywheelUp()
{
int speed = 0;

switch(LauncherRange)
{
case Far: { speed = 600; break; }
case Middle: { speed = 500; break; }
case Near: { speed = 800; break; }
case AutonomousMode: { speed = 450; break; }
case AutonomousModeShort: { speed = 0; break; }
case WarmUP: { speed = 400; break; }
case WarmUP2: { speed = 600; break; }
case Test: { speed = 400; break; }
default: { speed = 850; break; }
}

//writeDebugStreamLine("Speed Up: %d", speed);
return speed;
}

int GetExpectedSpeedFlywheelDown()
{
int speed = 0;

switch(LauncherRange)
{
case Far: { speed = 60; break; }
case Middle: { speed = 50; break; }
case Near: { speed = 80; break; }
case AutonomousMode: { speed = 450; break; }
case AutonomousModeShort: { speed = 0; break; }
case WarmUP: { speed = 40; break; }
case WarmUP2: { speed = 60; break; }
case Test: { speed = 40; break; }
default: { speed = 85; break; }
}

//writeDebugStreamLine("Speed Down: %d", speed);
return speed;
}
*/
int GetPowerFlywheelUp()
{
	int power = 0;

	switch(LauncherRange)
	{
	case Far: { power = 64; break; }
	case Middle: { power = 50; break; }
	case Near: { power = 50; break; }
	case AutonomousMode: { power = 63; break; }
	case AutonomousModeShort: { power = 0; break; }
	case Skill: { power = 73; break; } // Good(80,50)
		//case UserControlMode: { power = 65; break; } // Good > 8000 Verified
	case UserControlMode: { power = 64; break; } // Mid >= 7800
		//case UserControlMode: { power = 61; break; } // Low
	case WarmUP: { power = 35; break; }
	case WarmUP2: { power = 60; break; }
	case Test: { power = 40; break; }
	default: { power = 85; break; }
	}

	if (LauncherRange == (GameMode)UserControlMode)
	{
		if (nImmediateBatteryLevel >= 7900)
		{
			power += 1;
		}
	}

	int newPower = power + (power * LauncherPowerOffset * 0.05);
	int adjustedPower = AdjustPowerUsingBatteryLevel(newPower);

	if (false && LauncherPowerOffset != 0)
	{
		writeDebugStreamLine("adjustedPower UP: LauncherPowerOffset: %d, power: %d -> %d", LauncherPowerOffset, power, newPower);
	}
	return adjustedPower;
}

int GetPowerFlywheelDown()
{
	int power = 0;

	switch(LauncherRange)
	{
	case Far: { power = 65; break; }
	case Middle: { power = 50; break; }
	case Near: { power = 50; break; }
	case AutonomousMode: { power = 64; break; }
	case AutonomousModeShort: { power = 75; break; }
	case Skill: { power = 73; break; }
		//case UserControlMode: { power = 66; break; } // Best >= 8000 Verified
	case UserControlMode: { power = 65; break; } // Mid >= 78
		//case UserControlMode: { power = 63; break; } // Low
	case WarmUP: { power = 35; break; }
	case WarmUP2: { power = 60; break; }
	case Test: { power = 40; break; }
	default: { power = 0; break; }
	}

	if (LauncherRange == (GameMode)UserControlMode)
	{
		if (nImmediateBatteryLevel >= 7900)
		{
			power += 1;
		}
	}

	int newPower = power + (power * LauncherPowerOffset * 0.05);
	int adjustedPower = AdjustPowerUsingExternalBatteryLevel(power) ;

	if (false && LauncherPowerOffset != 0)
	{
		writeDebugStreamLine("adjustedPower Down: LauncherPowerOffset: %d, power: %d -> %d", LauncherPowerOffset, power, newPower);
	}
	return adjustedPower;

	/*
	float offset = LauncherPowerOffset * 80000;
	power = AdjustPowerUsingExternalBatteryLevel(power) ;
	int adjustedPower = (power * offset) / 80000;

	writeDebugStreamLine(",	adjustedPower Down: %d, PowerRaw: %d ", adjustedPower, power);

	return adjustedPower;
	*/
	/*
	float offset = LauncherPowerOffset * 100000;
	power = (power * offset) / 100000;
	int adjustedPower = AdjustPowerUsingExternalBatteryLevel(power) ;

	writeDebugStreamLine(",	adjustedPower Down: %d, PowerRaw: %d ", adjustedPower, power);

	return adjustedPower;
	*/
}

int GetMidBeltPower()
{
	int power = 0;

	switch(BeltSpeed)
	{
	case Fast: { power = 28; break; }
	case Slow: { power = 25; break; }
	case Skill: { power = 32; break; }
	case UserControlMode: { power = 25; break; }
	case AutonomousMode: { power = 25; break; }
	default: { power = 28; break; }
	}

	int adjustedPower = AdjustPowerUsingBatteryLevel(power);
	return adjustedPower;
}

int GetBeltPower()
{
	int power = 0;

	switch(BeltSpeed)
	{
	case Fast: { power = 50; break; }
	case Slow: { power = 40; break; }
	case Skill: { power = 40; break; }
	case UserControlMode: { power = 45; break; }
	case AutonomousMode: { power = 30; break; }
	case AutonomousModeShort: { power = 30; break; }
	default: { power = 35; break; }
	}

	int adjustedPower = AdjustPowerUsingExternalBatteryLevel(power);
	return adjustedPower;
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

/*
float AdjustLauncherPower()
{
float launcherUp = GetEncoderLauncherUp();
float launcherDown = GetEncoderLauncherDown();

float error = launcherUp - launcherDown;

float value = 1;

if (error > 0)
{
value = launcherUp / launcherDown;

//value = error / launcherDown;
}
else if (error < 0)
{
value = launcherUp / launcherDown;

//value = error / launcherDown;
}

//writeDebugStreamLine("Up: %d, Down: %d, value: %f", launcherUp, launcherDown, value);

return value;
}
*/

int GetEncoderLauncherUp()
{
	return abs(nMotorEncoder(LauncherUp1));
}

int GetEncoderLauncherDown()
{
	return abs(nMotorEncoder(LauncherDown1));
}

void ClearEncoder()
{
	nMotorEncoder(LauncherUp1) = 0;
	nMotorEncoder(LauncherDown1) = 0;
}
/*
void ClearEncoderUp()
{
nMotorEncoder(LauncherUp1) = 0;
}

void ClearEncoderDown()
{
nMotorEncoder(LauncherDown1) = 0;
}
*/
task usercontrol()
{
	// User control code here, inside the loop
	// int isLauncherRunning = false;
	// int isBeltRunning = false;
	LauncherRange = Near;
	BeltSpeed	= Fast;
	LauncherPowerOffset = 0;

	while (true)
	{
		wait1Msec(50);

		int btnLCD = nLCDButtons;
		if (btnLCD >= 1)
	  {
			startTask(displayBatteryLevelOnLCD);
		}

		// Game Mode
		int btn5u = vexRT[Btn5U]; // start user control
		int btn7u = vexRT[Btn7U]; // Launcher start for short game
		int btn7d = vexRT[Btn7D]; // Launcher start for long game
		int btn5d = vexRT[Btn5D]; // stop game mode / user control

		int btn6u = vexRT[Btn6U]; // start autonomous



		//int btn6u = vexRT[Btn6U]; // start belt
		//int btn6d = vexRT[Btn6D]; // stop belt

		// Launcher Speed
		int btn8u = vexRT[Btn8U]; // Fast Launcher
		int btn8d = vexRT[Btn8D]; // Slow Launcher

		// Launcher Speed
		int btn8l = vexRT[Btn8L]; // Start Belt auto
		int btn8r = vexRT[Btn8R]; // Stop/Reverser Belt

		int btn6d = vexRT[Btn6D]; // Start Belt Manual


		// Move
		int power3 = vexRT[Ch3];  // Move forward, backward
		int power1 = vexRT[Ch1]; // Rotate

		//int power2 = vexRT[Ch2];  // Move forward, backward
		//int power4 = vexRT[Ch4]; // Rotate
		int btn7l = vexRT[Btn7L]; // Shift left
		int btn7r = vexRT[Btn7R]; // Shift right
		int power7 = 110;

		motor[MiddleWheel] = (btn7r - btn7l) * power7;
		motor[WheelRight] = power3 - power1;
		motor[WheelLeft] = power3 + power1;


		//if (LauncherRange == (int)Far && SensorValue[touchSensorLoaded] == 1 && time1[T4] > 1000)
		//{
		//	CheckAndAdjust();
		//	clearTimer(T4);
		//}

		if (btn5u == 1)
		{
			startTask(launchBall_UserControl);
		}
		else if (btn5d == 1)
		{
			startTask(stopGameMode);
		}
		else if (btn6u == 1)
		{
			startTask(autonomous);
		}
		else if (btn7u == 1)
		{
			LauncherPowerOffset = 0;
			LauncherRange = Near;
			BeltSpeed = Slow;

			startTask(startShortGameMode);
			// startTask(startLauncher);
			//StartAndControlLauncher();
		}
		else if (btn7d == 1)
		{
			//LauncherRange = Test;

			LauncherPowerOffset = 0;
			//startTask(autonomous);
			////LauncherRange = Far;
			LauncherRange = UserControlMode;
			BeltSpeed	 = UserControlMode;

			startTask(startGameMode);
			//startTask(startLauncher);
			//StartAndControlLauncher();
		}
		else if (btn8u == 1 && time1[T3] > 400)
		{
			writeDebugStreamLine("Up Pressed!");
			clearTimer(T3);

			float powerUp = motor[LauncherUp1];
			float powerDown = motor[LauncherDown1];

			if (powerUp + powerDown > 0)
			{
				if (true)
				{
					LauncherPowerOffset++;
					startTask(startLauncher);
				}
				else
				{
					float powerUpNew = powerUp * 1.05;
					float powerDownNew = powerDown * 1.05;

					writeDebugStreamLine("^^^^^^^^^^^ Speed Up %d -> %d", powerUp, powerUpNew);


					motor[LauncherUp1] = powerUpNew;
					motor[LauncherUp2] = powerUpNew;
					motor[LauncherDown1] = powerDownNew;
					motor[LauncherDown2] = powerDownNew;
				}
			}
		}
		else if (btn8d == 1 && time1[T3] > 400)
		{
			writeDebugStreamLine("Down Pressed!");
			clearTimer(T3);

			float powerUp = motor[LauncherUp1];
			float powerDown = motor[LauncherDown1];

			if (powerUp + powerDown > 0)
			{
				if (true)
				{
					LauncherPowerOffset--;
					startTask(startLauncher);
				}
				else
				{
					float powerUpNew = powerUp * 0.95;
					float powerDownNew = powerDown * 0.95;

					writeDebugStreamLine("````````````` Speed Down %d -> %d", powerUp, powerUpNew);

					motor[LauncherUp1] = powerUpNew;
					motor[LauncherUp2] = powerUpNew;
					motor[LauncherDown1] = powerDownNew;
					motor[LauncherDown2] = powerDownNew;
				}
			}
			//BeltSpeed = Slow;
			//startTask(startBelt);
		}
		else if (btn8r == 1)
		{
			StopOrReverseBelt();
		}

		else if (btn8l == 1)
		{
			motor[TopBelt] = motor[BottomBelt] = 50 * btn8l;
			/*
			startTask(startBelt);

			startTask(startMidBelt);

			int power = motor[LauncherUp1];
			if (power > 0)
			{
			writeDebugStreamLine(">>>>>>>>>>>>>> Launched!!!");

			ClearEncoder();
			wait1Msec(200);
			float launcherUp = GetEncoderLauncherUp();
			float launcherDown = GetEncoderLauncherDown();
			AdjustSkill(launcherUp, launcherDown);
			}*/

		}
		else if (btn6d == 1)
		{
			motor[TopBelt] = motor[BottomBelt] = GetBeltPower();
			motor[MidBelt] = GetMidBeltPower();

			//if (LauncherRange == (int)Far)
			//{
			//	CheckAndAdjust();
			//}
		}

		//else if (btn6d == 0)
		//{
		//	motor[TopBelt] = motor[BottomBelt] = 0;
		//	motor[MidBelt] = 0;
		//}
	}
}

void StopOrReverseBelt()
{
	int power = motor[TopBelt];

	if (power != 0)
	{
		startTask(stopBelt);
	}
	else
	{

		motor[MidBelt] = 0;
		motor[TopBelt] = motor[BottomBelt] = 0;

		motor[MidBelt] = -80;
		motor[TopBelt] = motor[BottomBelt] = -25;

		wait1Msec(150);

		startTask(stopBelt);
	}
}

void WarmUpLauncher()
{
	float power = motor[LauncherDown1];
	int power_Down = GetPowerFlywheelDown();
	if (power < power_Down)
	{
		//writeDebugStream("WarmUP started\t");

		LauncherRange = WarmUP;
		startTask(startLauncher);


		//writeDebugStream("WarmUP#1\t");

		wait1Msec(1500);

		LauncherRange = WarmUP2;
		startTask(startLauncher);

		//writeDebugStream("WarmUP#2\t");

		wait1Msec(1000);
	}

	//writeDebugStreamLine("\tWarmUP done\t");
}
/*
void StartAndControlLauncher()
{
int launcherValue = LauncherRange;
WarmUpLauncher();

LauncherRange = launcherValue;

LauncherPowerOffset = 0;
startTask(startLauncher);
//startTask(testTask);
clearTimer(T2);

int powerOffset = 0;
while(true)
{
int btn8l = vexRT[Btn8L]; // MidBelt
int btn8u = vexRT[Btn8U]; // Launcher Power Increase
int btn8d = vexRT[Btn8D]; // Launcher Power Decrease
//int btn8r = vexRT[Btn8R]; // Stop

//int btn7r = vexRT[Btn7R]; // stop
int btn5d = vexRT[Btn5D]; // stop launcher
int btn6d = vexRT[Btn6D]; // stop launcher

motor[MidBelt] = 30 * btn8l;

if (btn5d + btn6d > 0)
{
break;
}

if (btn8u > 0)
{
powerOffset++;
}
else if (btn8d > 0)
{
powerOffset--;
}

}

startTask(stopGameMode);
}
*/
