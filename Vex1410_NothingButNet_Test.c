#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    ExternalBatteryValue, sensorAnalog)
#pragma config(Sensor, dgtl1,  touchSensorLaunched, sensorTouch)
#pragma config(Sensor, dgtl2,  touchSensorLoaded, sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           MidBelt,       tmotorVex393, openLoop)
#pragma config(Motor,  port2,           LauncherUp1,   tmotorVex393, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port3,           LauncherUp2,   tmotorVex393, openLoop)
#pragma config(Motor,  port4,           LauncherDown1, tmotorVex393, openLoop, reversed, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port5,           LauncherDown2, tmotorVex393, openLoop)
#pragma config(Motor,  port6,           BottomBelt,    tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port7,           TopBelt,       tmotorVex393, openLoop)
#pragma config(Motor,  port8,           WheelRight,    tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port9,           WheelLeft,     tmotorVex393, openLoop)
#pragma config(Motor,  port10,          MiddleWheel,   tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// #pragma config(Motor,  port9,           WheelBackRight, tmotorVex393, openLoop)
// #pragma config(Motor,  port10,          BeltUp,        tmotorVex393, openLoop)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(10)
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

task adjustLauncher();
task adjustLauncherUp();
task adjustLauncherDown();

task adjustUp();
task adjustDown();

//void AdjustUp();
//void AdjustDown();

//task testTask();

/// global variables
// Lancher Range Mode
int LauncherRange;

// Lancher Range Mode
int BeltSpeed;

float LauncherPowerOffset;

bool AdjustedUp;
bool AdjustedDown;

//bool IsLauncherRunning;

// global subroutines
void WarmUpLauncher();
void MakeLauncherIdle();
int GetPowerFlywheelUp();
int GetPowerFlywheelDown();
int GetBeltPower();
int GetMidBeltPower();
void AdjustSkill(float up, float down);
int AdjustPowerUsingBatteryLevel(int originalPower);
int AdjustPowerUsingExternalBatteryLevel(int originalPower);
void StartAndControlLauncher();
int GetEncoderLauncherUp();
int GetEncoderLauncherDown();
float AdjustLauncherPower();
void ClearEncoder();
void ClearEncoderUp();
void ClearEncoderDown();
int GetExpectedSpeedFlywheelUp();
int GetExpectedSpeedFlywheelDown();

// properties
enum {
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
	WarmUpLauncher();
	//wait1Msec(1000);

	LauncherRange = AutonomousMode;
	BeltSpeed = AutonomousMode;

	StartTask(startLauncher);
	wait1Msec(1500);

	StartTask(startBelt);
	StartTask(startMidBelt);

	wait1Msec(10000);

	StartTask(stopLauncher);
	StartTask(stopBelt);
}


task launchBall()
{
	LauncherRange = Skill;
	BeltSpeed = Skill;

	StartTask(startLauncher);
	wait1Msec(1000);

	StartTask(startBelt);
	StartTask(startMidBelt);

	float launcherUp;
	float launcherDown;
			
	ClearTimer(T1);
	ClearEncoder();
	ClearTimer(T2);
	//ClearTimer(T3);

	bool loaded = false;
	int index = 0;
	while(true)
	{
		//if (time1[T3] >= 200)
		//{
		//	launcherUp = GetEncoderLauncherUp();
		//	launcherDown = GetEncoderLauncherDown();
		//	writeDebugStreamLine("Up: %d, Down: %d", launcherUp, launcherDown);
			
		//	ClearTimer(T1);
		//	ClearEncoder();
		//}
	
		if (time1[T1] >= 1500 && SensorValue[touchSensorLoaded] == 1)
		{
			loaded = true;
			//wait1Msec(30);

		  //writeDebugStreamLine(">>>>>>>>> Launched");
			
			//StartTask(startLauncher);
			//MakeLauncherIdle();
			//ClearTimer(T1);
			//ClearTimer(T2);
			
		  ClearTimer(T3);
			StartTask(startLauncher);

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

			ClearTimer(T1);
			ClearTimer(T2);
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
			StartTask(startLauncher);


			//writeDebugStreamLine("<<<<<<<<< Idle Launcher Started");
			MakeLauncherIdle();
			writeDebugStreamLine("<<<<<<<<< Idle Launcher Done");
						
			ClearTimer(T2);
		}*/
		
	}
}

void AdjustSkill(float up, float down)
{
	float powerUp = motor[LauncherUp1];
	float powerDown = motor[LauncherDown1];
	
	bool adjusted = false;
	//if (up < 95 || 99 < up)
	if (up < 95)
	{
		adjusted = true;
		float powerUpNew = powerUp * 1.2;
		motor[LauncherUp1] = powerUpNew;
		motor[LauncherUp2] = powerUpNew;
	}
	else if (99 < up)
	{
		adjusted = true;
		float powerUpNew = powerUp * 0.8;
		motor[LauncherUp1] = powerUpNew;
		motor[LauncherUp2] = powerUpNew;
	}
	
	if (down < 105)
	{
		adjusted = true;
		float powerDownNew = powerDown * 1.2;
		motor[LauncherDown1] = powerDownNew;
		motor[LauncherDown2] = powerDownNew;
	}
	else if (110 < down)
	{
		adjusted = true;
		float powerDownNew = powerDown * 0.8;
		motor[LauncherDown1] = powerDownNew;
		motor[LauncherDown2] = powerDownNew;
	}
	
	
	if (adjusted == true)
	{
		writeDebugStreamLine("---------- Adjust Skill done");
	
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

task launchBall_DrivingMode()
{
	// Made 13-16 BALLS
	LauncherRange = UserControlMode;
	BeltSpeed = UserControlMode;

	StartTask(startLauncher);
	wait1Msec(1000);

	StartTask(startBelt);
	StartTask(startMidBelt);

	int launcherUp;
	int launcherDown;
			
	ClearTimer(T1);
	ClearEncoder();

	
	ClearTimer(T2);
	
	while(true)
	{/*
		if (time1[T1] >= 1000)
		{
			launcherUp = GetEncoderLauncherUp();
			launcherDown = GetEncoderLauncherDown();
			writeDebugStreamLine("Up: %d, Down: %d", launcherUp, launcherDown);
			
			ClearTimer(T1);
			ClearEncoder();
		}*/
	
		if (SensorValue[touchSensorLaunched] == 1)
		{
			wait1Msec(30); // Need this to avoid noise.

			//StartTask(startLauncher);
			//MakeLauncherIdle();
			//ClearTimer(T2);
			
			StartTask(startLauncher);

			int powerMidBelt = motor[MidBelt];
			int powerTopBelt = motor[TopBelt];
			
			motor[MidBelt] = 0;
			motor[TopBelt] = motor[BottomBelt] = 0;
			
			
			wait1Msec(1500);
			writeDebugStreamLine(">>>>>>>>> Launched: Up: %d, Down: %d", launcherUp, launcherDown);
			ClearTimer(T2);
			
			motor[MidBelt] = powerMidBelt;
			motor[TopBelt] = motor[BottomBelt] = powerTopBelt;
		}
		
		
		if (time1[T2] >= 3000)
		//if (false)
		{
			StartTask(startLauncher);


			//writeDebugStreamLine("<<<<<<<<< Idle Launcher Started");
			MakeLauncherIdle();
			writeDebugStreamLine("<<<<<<<<< Idle Launcher Done");
						
			ClearTimer(T2);
		}
		
	}
}



void MakeLauncherIdle()
{
	int powerMidBelt = motor[MidBelt];
	int powerTopBelt = motor[TopBelt];
	motor[MidBelt] = 0;
	motor[TopBelt] = motor[BottomBelt] = 0;
		
	StopTask(startMidBelt);

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



task launchBall_Skill()
{
	LauncherRange = Skill;
	BeltSpeed = Skill;

	StartTask(startLauncher);
	wait1Msec(700);

	StartTask(startBelt);
	StartTask(startMidBelt);

	int launcherUp;
	int launcherDown;

			
	ClearTimer(T1);
	ClearEncoder();

	while(true)
	{
		if (time1[T1] >= 1000)
		{
			launcherUp = GetEncoderLauncherUp();
			launcherDown = GetEncoderLauncherDown();
			writeDebugStreamLine("Up: %d, Down: %d", launcherUp, launcherDown);
			
			ClearTimer(T1);
			ClearEncoder();
		}
	
		if (SensorValue[touchSensorLaunched] == 1)
		{
				wait1Msec(50);
				writeDebugStreamLine(">>>>>>>>> Launched: Up: %d, Down: %d", launcherUp, launcherDown);
		}
	}
}


task launchBall_Old()
{
	/*
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10000;j++)
		{
		}
		writeDebugStreamLine("%d", i);
	}
	*/
	
	LauncherRange = AutonomousMode;
	AdjustedUp = false;
	AdjustedDown = false;

		ClearTimer(T1);

		StartTask(adjustUp);
		StartTask(adjustDown);

		//do{}
		while(AdjustedUp == false || AdjustedDown == false){};

		writeDebugStreamLine("TImer---------->   %d", time1[T1]);

		StartTask(startBelt);
		StartTask(startMidBelt);

		writeDebugStreamLine(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Launched ");

		while(SensorValue[touchSensorLaunched] == 0){};

		StartTask(stopBelt);

		while(true)
		{
			StartTask(adjustUp);
			StartTask(adjustDown);
		}
		
		

		//StartTask(adjustUp);
		//StartTask(adjustDown);

}

task startGameMode()
{
	LauncherPowerOffset = 1;

	LauncherRange = WarmUP;
	StartTask(startLauncher);

	wait1Msec(1000);

	LauncherRange = AutonomousMode;
	BeltSpeed = AutonomousMode;

	StartTask(startLauncher);

	wait1Msec(500);

	StartTask(startBelt);

	//LauncherRange = MidBeltPower;
	StartTask(startMidBelt);

	ClearTimer(T2);
	//ClearTimer(T4);
	ClearEncoder();

	//float launcherUp;
	//float launcherDown;

	while(true)
	{

		if (time1[T2] > 1000)
		{
			StartTask(adjustUp);
			StartTask(adjustDown);

			ClearTimer(T2);
			//ClearEncoder();

		}


		//AdjustUp();
		//AdjustDown();


		int btn5d = vexRT[Btn5D]; // stop launcher
		if (btn5d == 1)
		{
			break;
		}

		int value = SensorValue[touchSensorLaunched];
		if (value > 0 && time1[T2] > 1000)
		{
			//StartTask(adjustLauncher);
			//ClearTimer(T4);
			//writeDebugStreamLine(">>>>> launched <<<<<");
			//writeDebugStreamLine("\tlauncherUp: %d, launcherDown: %d", launcherUp, launcherDown);
		}
	}

	StopTask(adjustUp);
	StopTask(adjustDown);

	StartTask(stopGameMode);
	StopTask(startGameMode);
}

//void AdjustUp()
task adjustUp()
{
	int target = GetExpectedSpeedFlywheelUp();
	int index = 0;
	AdjustedUp = false;
	ClearEncoderUp();

	writeDebugStreamLine(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ajustUp");


	while(index < 10)
	{
		int btn5d = vexRT[Btn5D]; // stop launcher
		if (btn5d == 1)
		{
			break;
		}

		ClearTimer(T3);
		while(time1[T3] < 1000){};

		//wait1Msec(200);

		int current = GetEncoderLauncherUp();
		float offset = (current - target);

		if (-5 <= offset && offset <= 5)
		{
			writeDebugStreamLine("(AdjustUp) current = target: current: %d -> target: %d", current, target);
			break;
		}
		else
		{
			index++;
			offset = (offset / 6) * 1.0;
			float power = motor[LauncherUp1];
			float newPower = power + (offset * -1);

			ClearEncoderUp();

			motor[LauncherUp1] = newPower;
			motor[LauncherUp2] = newPower;

			writeDebugStreamLine("(>>>>> AdjustUp index:%d) current: %d -> target: %d, Power: %d -> newPower: %d", index, current, target, power, newPower);
		}
	}

	AdjustedUp = true;
	StopTask(adjustUp);
}

// void AdjustDown()
task adjustDown()
{
	int target = GetExpectedSpeedFlywheelDown();
	int index = 0;
	AdjustedDown = false;
	ClearEncoderDown();

	writeDebugStreamLine("<<<<<<<<<<<<<<<<<<<<<<<<< ajustDown");


	while(index < 10)
	{
		int btn5d = vexRT[Btn5D]; // stop launcher
		if (btn5d == 1)
		{
			break;
		}

		ClearTimer(T4);
		while(time1[T4] < 1000){};

		//wait1Msec(200);

		int current = GetEncoderLauncherDown();
		float offset = (current - target);

		if (-5 <= offset && offset <= 5)
		{
			writeDebugStreamLine("(AdjustDown) current = target: current: %d -> target: %d", current, target);
			break;
		}
		else
		{
			index++;
			offset = (offset / 6) * 1.0;
			float power = motor[LauncherDown1];
			float newPower = power + (offset * -1);

			ClearEncoderDown();

			motor[LauncherDown1] = newPower;
			motor[LauncherDown2] = newPower;

			writeDebugStreamLine("(>>>>> AdjustDown index:%d) current: %d -> target: %d, Power: %d -> newPower: %d", index, current, target, power, newPower);
		}
	}

	AdjustedDown = true;
	StopTask(adjustDown);
}

/*
void AdjustUp(int current)
{
	int target = GetExpectedSpeedFlywheelUp() / 10;
	current = current / 10;
	float offset = current - target;

	if (-2 <= offset && offset <= 2)
	{
		//writeDebugStreamLine("(AdjustUp) current = target: target: %d, current: %d", target, current);
	}
	else if (offset < 0)
	{
		offset = offset * 0.5;
		float power = motor[LauncherUp1];
		float newPower = power + (offset * -1);

		motor[LauncherUp1] = newPower;
		motor[LauncherUp2] = newPower;
		//writeDebugStreamLine("(AdjustUp) current < target: Power: %d, newPower: %d", power, newPower);
	}
	else if (offset > 0)
	{
		offset = offset * 0.5;
		float power = motor[LauncherUp1];
		float newPower = power + (offset * -1);

		motor[LauncherUp1] = newPower;
		motor[LauncherUp2] = newPower;
		//writeDebugStreamLine("(AdjustUp) current > target: Power: %d, newPower: %d", power, newPower);
	}
}

void AdjustDown(int current)
{
	int target = GetExpectedSpeedFlywheelDown() / 10;
	current = current / 10;
	float offset = current - target;

	if (-2 <= offset && offset <= 2)
	{
		//writeDebugStreamLine("(AdjustDown) current = target: target: %f, current: %f", target, current);
	}
	else if (offset < 0)
	{
		offset = offset * 0.5;
		float power = motor[LauncherDown1];
		float newPower = power + (offset * -1);

		motor[LauncherDown1] = newPower;
		motor[LauncherDown2] = newPower;
		//writeDebugStreamLine("(AdjustDown) current < target: Power: %f, newPower: %f", power, newPower);
	}
	else if (offset > 0)
	{
		offset = offset * 0.5;
		float power = motor[LauncherDown1];
		float newPower = power + (offset * -1);

		motor[LauncherDown1] = newPower;
		motor[LauncherDown2] = newPower;
		//writeDebugStreamLine("(AdjustDown) current > target: Power: %f, newPower: %f", power, newPower);
	}
}
*/


task adjustLauncher()
{
	StartTask(adjustLauncherUp);
	StartTask(adjustLauncherDown);
}

task adjustLauncherUp()
{
	float launcherCurrent = GetEncoderLauncherUp();
	int target = GetExpectedSpeedFlywheelUp();

	if (launcherCurrent < target)
	{
		int launcherPrevious = GetEncoderLauncherUp();

		float power = GetPowerFlywheelUp();
		float newPower = power * 1.1;

		motor[LauncherUp1] = newPower;
		motor[LauncherUp2] = newPower;
		wait1Msec(500);
		launcherCurrent = GetEncoderLauncherUp();

		writeDebugStreamLine("launcherPrevious: %d -> launcherCurrent: %d", launcherPrevious, launcherCurrent);
	}
	else if (launcherCurrent > target)
	{
		writeDebugStreamLine("launcherCurrent > target");
	}
	else
	{
		writeDebugStreamLine("launcherCurrent == target");
	}
}

task adjustLauncherDown()
{
	float launcherCurrent = GetEncoderLauncherDown();
	int target = GetExpectedSpeedFlywheelDown();

	if (launcherCurrent < target)
	{
		int launcherPrevious = GetEncoderLauncherDown();

		float power = GetPowerFlywheelDown();
		float newPower = power * 1.1;

		motor[LauncherUp1] = newPower;
		motor[LauncherUp2] = newPower;
		wait1Msec(500);
		launcherCurrent = GetEncoderLauncherDown();

		writeDebugStreamLine("launcherPrevious: %d -> launcherCurrent: %d", launcherPrevious, launcherCurrent);
	}
	else if (launcherCurrent > target)
	{
		writeDebugStreamLine("launcherCurrent > target");
	}
	else
	{
		writeDebugStreamLine("launcherCurrent == target");
	}
}

/*
task adjustLauncherUp()
{
	float launcherCurrent = GetEncoderLauncherUp();
	int target = 600;
	int offset = 0;

		if (launcherCurrent < target)
		{
			int index = 0;
			while(launcherCurrent + offset < target)
			{
				launcherPrevious = GetEncoderLauncherUp();
				float power = motor[LauncherUp2];
				power = power * 1.02;

				motor[LauncherUp2] = power;
				wait1Msec(500);
				launcherCurrent = GetEncoderLauncherUp();
				int offset = launcherCurrent - launcherPrevious;

				writeDebugStreamLine("launcherPrevious: %d -> launcherCurrent: %d, offset: %d, index: %d", launcherPrevious, launcherCurrent, offset, index);
			}

			//value = error / launcherDown;
		}
		else if (launcherCurrent > target)
		{
			value = launcherUp / launcherDown;

			//value = error / launcherDown;
		}
		else
		{

		}
	}
	//writeDebugStreamLine("Up: %d, Down: %d, value: %f", launcherUp, launcherDown, value);
}
*/
task startShortGameMode()
{
	LauncherRange = AutonomousModeShort;
	BeltSpeed = AutonomousModeShort;
	LauncherPowerOffset = 1;

	StartTask(startLauncher);

	wait1Msec(2000);

	StartTask(startBelt);
}

task stopGameMode()
{
	StartTask(stopLauncher);
	StartTask(stopBelt);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
/*
task testTask()
{
}
*/

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

int GetPowerFlywheelUp()
{
	int power = 0;

	switch(LauncherRange)
	{
	case Far: { power = 60; break; }
	case Middle: { power = 50; break; }
	case Near: { power = 80; break; }
	case AutonomousMode: { power = 70; break; }
	case AutonomousModeShort: { power = 0; break; }
	case Skill: { power = 65; break; }
	case UserControlMode: { power = 68; break; }
	case WarmUP: { power = 40; break; }
	case WarmUP2: { power = 60; break; }
	case Test: { power = 40; break; }
	default: { power = 85; break; }
	}

	int adjustedPower = AdjustPowerUsingBatteryLevel(power);

	//writeDebugStream("adjustedPower UP: %d, powerRaw: %d", adjustedPower, power);

	return adjustedPower;
}

int GetPowerFlywheelDown()
{
	int power = 0;

	switch(LauncherRange)
	{
	case Far: { power = 55; break; }
	case Middle: { power = 50; break; }
	case Near: { power = 0; break; }
	case AutonomousMode: { power = 70; break; }
	case AutonomousModeShort: { power = 75; break; }
	case Skill: { power = 65; break; }
	case UserControlMode: { power = 68; break; }
	case WarmUP: { power = 40; break; }
	case WarmUP2: { power = 60; break; }
	case Test: { power = 40; break; }
	default: { power = 0; break; }
	}

	int adjustedPower = AdjustPowerUsingExternalBatteryLevel(power) ;

	//writeDebugStreamLine(",	adjustedPower Down: %d, PowerRaw: %d ", adjustedPower, power);

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
		case Skill: { power = 28; break; }
		case UserControlMode: { power = 28; break; }
		case AutonomousMode: { power = 20; break; }
		default: { power = 23; break; }
	}

	int adjustedPower = AdjustPowerUsingBatteryLevel(power);
	return adjustedPower;
}

int GetBeltPower()
{
	int power = 0;

	switch(BeltSpeed)
	{
	case Fast: { power = 60; break; }
	case Slow: { power = 30; break; }
	case Skill: { power = 30; break; }
	case UserControlMode: { power = 30; break; }
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
	//float batteryLevel = SensorValue[ExternalBatteryValue] * 3.57;
	float batteryLevel = SensorValue[ExternalBatteryValue] * 3.57;

	float batteryOffset =	7000 / batteryLevel;
	int adjustedPower = originalPower * batteryOffset;
	//  writeDebugStreamLine("(AjustBattery) BatterLevel: %d - OriginalPower: %d - WantedPower: %d", AdjustBatteryLevel, OriginalPower, WantedPower);
	return adjustedPower;
}

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

void ClearEncoderUp()
{
	nMotorEncoder(LauncherUp1) = 0;
}

void ClearEncoderDown()
{
	nMotorEncoder(LauncherDown1) = 0;
}

task usercontrol()
{
	// User control code here, inside the loop
	// int isLauncherRunning = false;
	// int isBeltRunning = false;
	LauncherRange = Middle;
	BeltSpeed	= Slow;
	LauncherPowerOffset = 1;

	bool HasGameModeStarted = false;

	bool warmedUp = false;
	
	while (true)
	{
		int btn5u = vexRT[Btn5U]; // start launcher
		int btn5d = vexRT[Btn5D]; // stop launcher

		int btn6u = vexRT[Btn6U]; // start belt
		int btn6d = vexRT[Btn6D]; // stop belt

		int btn7u = vexRT[Btn7U]; // short range
		int btn7d = vexRT[Btn7D]; // long
		
		int btn8u = vexRT[Btn8U]; // Fast Belt
		int btn8d = vexRT[Btn8D]; // Slow Belt
		int btn8r = vexRT[Btn8R]; // stop Belt
		int btn8l = vexRT[Btn8L]; // stop Belt

		
		// moving
		int btn7l = vexRT[Btn7L]; // Shift left
		int btn7r = vexRT[Btn7R]; // Shift right
		int power7 = 110;

		int power1 = vexRT[Ch1]; //
		int power2 = vexRT[Ch2];  //Move forward, backward
		int power3 = vexRT[Ch3];  //
		int power4 = vexRT[Ch4]; //Rotate

		motor[MiddleWheel] = (btn7r - btn7l) * power7;
		motor[WheelRight] = power2 + power4;
		motor[WheelLeft] = power2 - power4;
	

		/*
		if (power1 > 0)
		{
			motor[WheelRight] = -power1;
			motor[WheelLeft] = power1;
		}
		if (power1 < 0)
		{
			motor[WheelRight] = -power1;
			motor[WheelLeft] = power1;
		}
		if (power1 == 0)
		{
			motor[WheelRight] = 0;
			motor[WheelLeft] = 0;
		}*/
	
			//if (power1 != 0)
			//{
			//}
			
			//if (power3 != 0)
			//{
			//	motor[WheelRight] = power3 - power4;
			//	motor[WheelLeft] = power3 + power4;
			//}
			//else if (power4 != 0)
			//{
			//	motor[WheelRight] = -power4;
			//	motor[WheelLeft] = power4;
			//}
			//else
			//{
			//	//motor[MiddleWheel] = 0;
	
			//	motor[WheelRight] = 0;
			//	motor[WheelLeft] = 0;
			//}
		/*
		if (power8L == 1)
		{
			motor[WheelMiddle] = 60;
		}
		if (power8R == 1)
		{
			//motor[WheelMiddle] = -60;
		}
		if (power8L == 0 && power8R == 0)
		{
			motor[WheelMiddle] = 0;
		}
		*/

		if (SensorValue[touchSensorLaunched] == 1)
		{
			if (HasGameModeStarted == true)
			{

			}
		}
		// start or stop launcher
		if (btn5u == 1)
		{
			HasGameModeStarted = true;
			StartTask(startGameMode);
		}

		if (btn5d == 1)
		{
			HasGameModeStarted = false;
			StartTask(stopGameMode);
		}

		// start or stop belt
		if (btn6u == 1)
		{
			//StartTask(startBelt);
			//StartTask(startShortGameMode);

			//if (warmedUp == false)
			//{
			//	warmedUp = true;
			//}

			WarmUpLauncher();

			//writeDebugStreamLine(">>>>>>>>>>>>>>>  launchBall invoked");

			StartTask(launchBall);
		}

		if (btn6d == 1)
		{
			//StartTask(stopGameMode);
			StopTask(launchBall);	
			StartTask(stopGameMode);
			

		}

		if (btn7u == 1)
		{
			LauncherRange = Near;
			//StartTask(startLauncher);
			StartAndControlLauncher();
		}

		/*
		if (btn7l == 1)
		{
			LauncherRange = Middle;
			//StartTask(startLauncher);
			StartAndControlLauncher();
		}
		
		if (btn7r == 1)
		{
			StartTask(stopLauncher);
		}
		*/

		if (btn7d == 1)
		{
			//LauncherRange = Test;

			LauncherRange = Far;
			//StartTask(startLauncher);
			StartAndControlLauncher();
		}

		if (btn8u == 1)
		{
			BeltSpeed = Fast;
			StartTask(startBelt);
		}

		if (btn8d == 1)
		{
			BeltSpeed = Slow;
			StartTask(startBelt);
		}

		if (btn8r == 1)
		{
			StartTask(stopBelt);
		}

		if (btn8l == 1)
		{
			StartTask(startMidBelt);
		}
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
		StartTask(startLauncher);


		//writeDebugStream("WarmUP#1\t");

		wait1Msec(1500);

		LauncherRange = WarmUP2;
		StartTask(startLauncher);

		//writeDebugStream("WarmUP#2\t");

		wait1Msec(1000);
	}
	
	//writeDebugStreamLine("\tWarmUP done\t");
}

void StartAndControlLauncher()
{
	int launcherValue = LauncherRange;
	WarmUpLauncher();

	LauncherRange = launcherValue;

	LauncherPowerOffset = 1;
	StartTask(startLauncher);
	//StartTask(testTask);
	ClearTimer(T2);

	while(true)
	{
		int btn7r = vexRT[Btn7R]; // stop
		int btn5d = vexRT[Btn5D]; // stop launcher

		if (btn7r + btn5d > 0)
		{
			break;
		}
		else
		{
			if (time1[T2] > 10000)
			{
				//LauncherPowerOffset =
				AdjustLauncherPower();
				//writeDebugStreamLine("LauncherPowerOffset: %f", LauncherPowerOffset);

				StartTask(startLauncher);
				//StartTask(testTask);
				ClearTimer(T2);
			}
		}
	}

	StartTask(stopLauncher);
}
