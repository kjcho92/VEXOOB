#pragma config(Sensor, in1,    Launcher1_Loader, sensorPotentiometer)
#pragma config(Sensor, dgtl1,  Launcher1_Ready, sensorTouch)
#pragma config(Sensor, dgtl2,  Launcher1_BallLoaded, sensorTouch)
#pragma config(Sensor, I2C_1,  Loader1_I2C,    sensorNone)
#pragma config(Motor,  port1,           Loader1,       tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           Launcher1_H,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           Launcher1_M,   tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           Launcher1_L,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           Belt,          tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          Launcher1_Wall, tmotorVex393_HBridge, openLoop, reversed)
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

	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task LaunchBall();
task LauncherUp();
task LauncherStop();
task LauncherDown();
task RotateLoader();


void LauncherDown_Helper(int count);
void LauncherStop_Helper();
void OpenLoader_Helper();
void CloseLoader_Helper();

bool Launcher_Loaded;

task usercontrol()
{
	// User control code here, inside the loop

	while (true)
	{

		int btn6u = vexRT[Btn6U]; // Fast Launcher
		int btn6d = vexRT[Btn6D]; // Fast Launcher

		int btn7u = vexRT[Btn7U]; // Fast Launcher
		int btn7d = vexRT[Btn7D]; // Slow Launcher

		int btn8u = vexRT[Btn8U]; // Fast Launcher
		int btn8d = vexRT[Btn8D]; // Slow Launcher
		int btn8l = vexRT[Btn8L]; // Slow Launcher
		int btn8r = vexRT[Btn8R]; // Slow Launcher

		//int power8 = 100; // shift

		//int powerLauncher = 90; // shift
		//int powerLoader = 12;

		////motor[Loader1] = -btn8d * powerLoader;
		//motor[Launcher1_H] = -btn8u * powerLauncher;
		//motor[Launcher1_M] = -btn8u * powerLauncher;
		//motor[Launcher1_L] = -btn8u * powerLauncher;


		int powerWall = 70;

		//motor[Loader1] = -btn8d * powerLoader;
		motor[Launcher1_Wall] = -btn7u * powerWall + btn7d * powerWall;


		int powerBelt = 70;
		motor[Belt] = -btn6u * powerBelt + btn6d * powerBelt;

		if (btn6u == 1)
		{
		}
		else if (btn8d == 1)
		{
			//stopTask(LauncherUp);

			//wait1Msec(10);
			startTask(LauncherDown);
		}
		else if (btn8l == 1)
		{
			//stopTask(LauncherUp);

			//wait1Msec(10);
			startTask(LaunchBall);
		}
		else if (btn8r == 1)
		{
			//stopTask(LauncherUp);

			//wait1Msec(10);
			startTask(RotateLoader);
		}
	}
}



task LaunchBall()
{//
	long startTime = nPgmTime;

	LauncherDown_Helper(4);

	writeDebugStreamLine("LaunchBall) Elapsed Time: %d", nPgmTime - startTime);
	startTask(LauncherStop);
}

task LaunchBall_01()
{//
	long prevTime = nPgmTime;
	long startTime = nPgmTime;
	for(int i=0;i<=3;i++)
	{
		LauncherDown_Helper(1);
		writeDebugStreamLine("LaunchBall) %d Time: %d", i, nPgmTime - prevTime);
		prevTime = nPgmTime;
	}

	writeDebugStreamLine("LaunchBall) Time: %d", nPgmTime - startTime);
	startTask(LauncherStop);

}


task LauncherDown()
{//
	LauncherDown_Helper(1);
}

task OpenLoader()
{//
	//waitUntil(SensorValue[Launcher1_Started] == 1);
	OpenLoader_Helper();
}

void OpenLoader_Helper()
{//
	//waitUntil(SensorValue[Launcher1_Started] == 1);

	int powerLoader = 100;
	int timeOut = 100;

	motor[Loader1] = powerLoader;
	//wait1Msec(70);

	unsigned long cTime = nPgmTime + timeOut;
	waitUntil(SensorValue[Launcher1_Loader] <= 1160 || cTime <= nPgmTime);
	//wait1Msec(100);

	motor[Loader1] = 0;
}


task CloseLoader()
{
	CloseLoader_Helper();
}

void CloseLoader_Helper()
{
	int powerLoader = 100;
	int timeOut = 100;

	motor[Loader1] = -powerLoader;
	//wait1Msec(100);

	unsigned long cTime = nPgmTime + timeOut;
	waitUntil(SensorValue[Launcher1_Loader] >= 3500 || cTime <= nPgmTime);
	motor[Loader1] = 0;
}

task RotateLoader()
{//
	//waitUntil(SensorValue[Launcher1_Started] == 1);

	int powerLoader = 100;
	int timeOut = 100;
	motor[Loader1] = powerLoader;
	wait1Msec(70);


	unsigned long cTime = nPgmTime + timeOut;


	waitUntil(SensorValue[Launcher1_Loader] <= 1160 || cTime <= nPgmTime);

	//wait1Msec(100);

	motor[Loader1] = 0;

	cTime = nPgmTime + 400;
	//waitUntil(cTime <= nPgmTime);
	waitUntil(SensorValue[Launcher1_BallLoaded] == 1 || cTime <= nPgmTime);
	//wait1Msec(70);

	motor[Loader1] = -powerLoader;
	wait1Msec(100);

	cTime = nPgmTime + timeOut;
	waitUntil(SensorValue[Launcher1_Loader] >= 3500 || cTime <= nPgmTime);
	motor[Loader1] = 0;


	//waitUntil(SensorValue[Launcher1_BallLoaded] == 0);


	//motor[Loader1] = powerLoader;

	//waitUntil(SensorValue[Launcher1_Loader] <= 1760);


	//motor[Loader1] = 0;

}

task RotateLoader_02()
{//
	//waitUntil(SensorValue[Launcher1_Started] == 1);

	nMotorEncoder(Loader1) = 0;

	int powerLoader = 35;
	motor[Loader1] = powerLoader;


	//Launcher_Loaded = false;
	waitUntil(abs(nMotorEncoder(Loader1)) > 48);


	writeDebugStreamLine("RotateLoader) #00 nMotorEncoder(Loader1) :%d", nMotorEncoder(Loader1));


	motor[Loader1] = 0;
	nMotorEncoder(Loader1) = 0;

	powerLoader = -50;
	motor[Loader1] = powerLoader;

	//Launcher_Loaded = false;
	waitUntil(abs(nMotorEncoder(Loader1)) > 20);

	motor[Loader1] = 0;

}

task RotateLoader_01()
{//
	//waitUntil(SensorValue[Launcher1_Started] == 1);

	int powerLoader = -40;
	motor[Loader1] = powerLoader;

	//nMotorEncoder(Loader1) = 0;

	//Launcher_Loaded = false;
	//waitUntil(SensorValue[Launcher1_Loaded] == 1);

	unsigned long cTime = nPgmTime + 1000;


	int released = 0;
	while(released == 0 && cTime >= nPgmTime)
	{
		//released = SensorValue[Launcher1_BallReleased];
	}


	writeDebugStreamLine("RotateLoader) #00 Launcher1_BallReleased :%d", released);

	//powerLoader = 80;
	//motor[Loader1] = powerLoader;

	//wait1Msec(80);

	motor[Loader1] = 0;

}

void LauncherDown_Helper(int count)
{
	int index = 1;
	int launcherPower = 100;
	int originalPower = launcherPower;

	startTask(OpenLoader);

	long startTime = nPgmTime;

	do
	{

		//startTask(RotateLoader);

		int primaryPower = launcherPower;

		motor[Launcher1_H] = primaryPower;
		motor[Launcher1_M] = primaryPower;
		motor[Launcher1_L] = primaryPower;

		//wait1Msec(100);

		waitUntil(SensorValue[Launcher1_Ready] == 1);

		writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Ready :%d", SensorValue[Launcher1_Ready]);

		unsigned long cTime = nPgmTime + 1000;

		int loaded = SensorValue[Launcher1_BallLoaded];

		while(loaded == 0 && cTime >= nPgmTime)
		{
			loaded = SensorValue[Launcher1_BallLoaded];

			originalPower = 20;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;


			writeDebugStreamLine("LauncherDown_Helper) #3-1 loaded:%d", loaded);

		}

		writeDebugStreamLine("LauncherDown_Helper) #3 loaded:%d", loaded);


		if (loaded == 1)
		{

			//wait1Msec(200);

			CloseLoader_Helper();

			originalPower = launcherPower;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;


			writeDebugStreamLine("LauncherDown_Helper) #4 index:%d, Elapsed Time: %d", index, nPgmTime - startTime);


			index++;


			//writeDebugStreamLine("LauncherDown_Helper) #4 loaded:%d", loaded);

			wait1Msec(150);
			if (count >= index)
			{
				startTask(OpenLoader);
			}
			wait1Msec(70);

			waitUntil(SensorValue[Launcher1_Ready] == 1);



			//waitUntil(SensorValue[Launcher1_Started] == 1);

			//writeDebugStreamLine("LauncherDown_Helper) #4-1 loaded:%d", SensorValue[Launcher1_Started]);

			//originalPower = 15;
			//primaryPower = originalPower;

			//motor[Launcher1_H] = primaryPower;
			//motor[Launcher1_M] = primaryPower;
			//motor[Launcher1_L] = primaryPower;
		}


		writeDebugStreamLine("LauncherDown_Helper) #5 Count:%d, index:%d", count, index);

	}
	while(count >= index);


	writeDebugStreamLine("LauncherDown_Helper) #6 Count:%d, index:%d", count, index);

	startTask(LauncherStop);
	startTask(CloseLoader);

	//stopTask(RotateLoader);

	return;
}


void LauncherDown_Helper_09(int count)
{
	int index = 1;
	int launcherPower = 100;
	int originalPower = launcherPower;

	startTask(RotateLoader);

	do
	{

		//startTask(RotateLoader);

		int primaryPower = launcherPower;

		motor[Launcher1_H] = primaryPower;
		motor[Launcher1_M] = primaryPower;
		motor[Launcher1_L] = primaryPower;

		//wait1Msec(100);

		waitUntil(SensorValue[Launcher1_Ready] == 1);

		writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Ready :%d", SensorValue[Launcher1_Ready]);

		unsigned long cTime = nPgmTime + 5000;

		int loaded = SensorValue[Launcher1_BallLoaded];

		while(loaded == 0 && cTime >= nPgmTime)
		{
			loaded = SensorValue[Launcher1_BallLoaded];

			originalPower = 20;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;


			writeDebugStreamLine("LauncherDown_Helper) #3-1 loaded:%d", loaded);

		}

		writeDebugStreamLine("LauncherDown_Helper) #3 loaded:%d", loaded);


		if (loaded == 1)
		{

			wait1Msec(200);

			originalPower = launcherPower;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;

			index++;


			writeDebugStreamLine("LauncherDown_Helper) #4 loaded:%d", loaded);

			wait1Msec(100);
			if (count >= index)
			{
				startTask(RotateLoader);
			}
			wait1Msec(100);

			waitUntil(SensorValue[Launcher1_Ready] == 1);


			//waitUntil(SensorValue[Launcher1_Started] == 1);

			//writeDebugStreamLine("LauncherDown_Helper) #4-1 loaded:%d", SensorValue[Launcher1_Started]);

			//originalPower = 15;
			//primaryPower = originalPower;

			//motor[Launcher1_H] = primaryPower;
			//motor[Launcher1_M] = primaryPower;
			//motor[Launcher1_L] = primaryPower;
		}


		writeDebugStreamLine("LauncherDown_Helper) #5 Count:%d, index:%d", count, index);

	}
	while(count >= index);


	writeDebugStreamLine("LauncherDown_Helper) #6 Count:%d, index:%d", count, index);

	startTask(LauncherStop);
	stopTask(RotateLoader);

	return;
}

void LauncherDown_Helper_08(int count)
{
	int index = 1;
	int launcherPower = 127;
	int originalPower = launcherPower;

	do
	{
		//startTask(RotateLoader);


		int primaryPower = launcherPower;

		motor[Launcher1_H] = primaryPower;
		motor[Launcher1_M] = primaryPower;
		motor[Launcher1_L] = primaryPower;

		//wait1Msec(100);

		waitUntil(SensorValue[Launcher1_Ready] == 1);

		writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Ready :%d", SensorValue[Launcher1_Ready]);

		unsigned long cTime = nPgmTime + 5000;

		int loaded = SensorValue[Launcher1_BallLoaded];

		while(loaded == 0 && cTime >= nPgmTime)
		{
			loaded = SensorValue[Launcher1_BallLoaded];

			originalPower = 15;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;


			writeDebugStreamLine("LauncherDown_Helper) #3-1 loaded:%d", loaded);

		}

		writeDebugStreamLine("LauncherDown_Helper) #3 loaded:%d", loaded);


		if (loaded == 1)
		{

			originalPower = launcherPower;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;

			index++;


			writeDebugStreamLine("LauncherDown_Helper) #4 loaded:%d", loaded);

			wait1Msec(100);
			waitUntil(SensorValue[Launcher1_Ready] == 1);

			//waitUntil(SensorValue[Launcher1_Started] == 1);

			//writeDebugStreamLine("LauncherDown_Helper) #4-1 loaded:%d", SensorValue[Launcher1_Started]);

			//originalPower = 15;
			//primaryPower = originalPower;

			//motor[Launcher1_H] = primaryPower;
			//motor[Launcher1_M] = primaryPower;
			//motor[Launcher1_L] = primaryPower;
		}


		writeDebugStreamLine("LauncherDown_Helper) #5 Count:%d, index:%d", count, index);

	}
	while(count >= index);


	writeDebugStreamLine("LauncherDown_Helper) #6 Count:%d, index:%d", count, index);

	startTask(LauncherStop);
	stopTask(RotateLoader);

	return;

}

void LauncherDown_Helper_07(int count)
{
	int index = 1;
	int launcherPower = 85;
	int originalPower = launcherPower;

	do
	{
		int primaryPower = launcherPower;

		motor[Launcher1_H] = primaryPower;
		motor[Launcher1_M] = primaryPower;
		motor[Launcher1_L] = primaryPower;


		//waitUntil(SensorValue[Launcher1_Started] == 1);

		//writeDebugStreamLine("LauncherDown_Helper) #1 Launcher1_Loaded :%d", SensorValue[Launcher1_Started]);

		startTask(RotateLoader);

		//waitUntil(SensorValue[Launcher1_BallReleased] == 1);


		writeDebugStreamLine("LauncherDown_Helper) #1-1 Launcher1_Ready :%d", SensorValue[Launcher1_Ready]);

		//wait1Msec(100);

		waitUntil(SensorValue[Launcher1_Ready] == 1);

		writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Ready :%d", SensorValue[Launcher1_Ready]);

		unsigned long cTime = nPgmTime + 1000;

		int loaded = SensorValue[Launcher1_BallLoaded];
		while(loaded == 0 && cTime >= nPgmTime)
		{
			loaded = SensorValue[Launcher1_BallLoaded];

			originalPower = 15;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;

		}

		writeDebugStreamLine("LauncherDown_Helper) #3 loaded:%d", loaded);


		if (loaded == 1)
		{

			originalPower = launcherPower;
			primaryPower = originalPower;

			motor[Launcher1_H] = primaryPower;
			motor[Launcher1_M] = primaryPower;
			motor[Launcher1_L] = primaryPower;

			index++;


			writeDebugStreamLine("LauncherDown_Helper) #4 loaded:%d", loaded);

			//waitUntil(SensorValue[Launcher1_Started] == 1);

			//writeDebugStreamLine("LauncherDown_Helper) #4-1 loaded:%d", SensorValue[Launcher1_Started]);

			//originalPower = 15;
			//primaryPower = originalPower;

			//motor[Launcher1_H] = primaryPower;
			//motor[Launcher1_M] = primaryPower;
			//motor[Launcher1_L] = primaryPower;
		}
	}
	while(count >= index);


	writeDebugStreamLine("LauncherDown_Helper) #5 Count:%d, index:%d", count, index);

	startTask(LauncherStop);
	stopTask(RotateLoader);

	return;
}

void LauncherDown_Helper_06()
{
	startTask(RotateLoader);

	int launcherPower = 85;
	int originalPower = launcherPower;
	int primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	//writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);

	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);

	unsigned long cTime = nPgmTime + 300;
	waitUntil(cTime <= nPgmTime);

	if (true)
	{
		waitUntil(SensorValue[Launcher1_Ready] == 1);
	}
	else
	{
		if (Launcher_Loaded == true)
		{
			waitUntil(SensorValue[Launcher1_Ready] == 1);
			//powerLoader = -120;
			//motor[Loader1] = powerLoader;

			//writeDebugStreamLine("LauncherDown_Helper) #4 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);
		}
	}
	startTask(LauncherStop);

}

void LauncherDown_Helper_04()
{
	int launcherPower = 90;
	int originalPower = launcherPower;
	int primaryPower = originalPower;


	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	waitUntil(SensorValue[Launcher1_Ready] == 1);


	int powerLoader = 30;
	motor[Loader1] = powerLoader;
	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 210);
	motor[Loader1] = 0;


	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);


	//powerLoader = 120;
	//motor[Loader1] = powerLoader;

	//nMotorEncoder(Loader1) = 0;
	//waitUntil(abs(nMotorEncoder(Loader1)) >= 50);
	//motor[Loader1] = 0;



	//writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);



	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);


	unsigned long cTime = nPgmTime + 300;
	waitUntil(cTime <= nPgmTime);

	if (SensorValue[Launcher1_BallLoaded] == 1)
	{
		waitUntil(SensorValue[Launcher1_Ready] == 1);

		//powerLoader = -120;
		//motor[Loader1] = powerLoader;

		//writeDebugStreamLine("LauncherDown_Helper) #4 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);
	}
	startTask(LauncherStop);

}

void LauncherDown_Helper_05()
{
	int launcherPower = 85;
	int originalPower = launcherPower;
	int primaryPower = originalPower;


	int powerLoader = -125;
	motor[Loader1] = powerLoader;

	nMotorEncoder(Loader1) = 0;


	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	waitUntil(abs(nMotorEncoder(Loader1)) >= 305);

	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);

	motor[Loader1] = 0;

	powerLoader = 120;
	motor[Loader1] = powerLoader;

	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 45);
	motor[Loader1] = 0;



	//writeDebugStreamLine("LauncherDown_Helper) #2 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);



	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);


	unsigned long cTime = nPgmTime + 200;
	waitUntil(cTime <= nPgmTime);

	if (true)
	{
		waitUntil(SensorValue[Launcher1_Ready] == 1);
	}
	else
	{
		if (SensorValue[Launcher1_BallLoaded] == 1)
		{
			waitUntil(SensorValue[Launcher1_Ready] == 1);
			//powerLoader = -120;
			//motor[Loader1] = powerLoader;

			//writeDebugStreamLine("LauncherDown_Helper) #4 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);
		}
	}
	startTask(LauncherStop);

}

void LauncherDown_Helper_02()
{
	int launcherPower = 83;
	int originalPower = launcherPower;
	int primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	waitUntil(SensorValue[Launcher1_Ready] == 1);

	int powerLoader = -120;
	motor[Loader1] = powerLoader;

	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 310);

	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);

	motor[Loader1] = 0;

	powerLoader = 110;
	motor[Loader1] = powerLoader;

	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 50);
	motor[Loader1] = 0;

	if (SensorValue[Launcher1_BallLoaded] == 1)
	{
		waitUntil(SensorValue[Launcher1_Ready] == 1);
	}
	startTask(LauncherStop);

	//waitUntil(SensorValue[Launcher1_Loaded] == 0);
	//writeDebugStreamLine("LauncherDown_Helper) #4 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);

}

void LauncherDown_Helper_01()
{//

	int launcherPower = 110;
	int originalPower = launcherPower;
	int primaryPower = originalPower;

	if (SensorValue[Launcher1_Ready] != 1)
	{
		motor[Launcher1_H] = primaryPower;
		motor[Launcher1_M] = primaryPower;
		motor[Launcher1_L] = primaryPower;

		//writeDebugStreamLine("LauncherDown_Helper) #1 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));

		waitUntil(SensorValue[Launcher1_Ready] == 1);

		//writeDebugStreamLine("LauncherDown_Helper) #2 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));


		originalPower = 15;
		primaryPower = originalPower;

		motor[Launcher1_H] = primaryPower;
		motor[Launcher1_M] = primaryPower;
		motor[Launcher1_L] = primaryPower;
	}

	int powerLoader = -90;
	motor[Loader1] = powerLoader;

	//waitUntil(SensorValue[Launcher1_Loaded] == 1)

	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 310);

	//writeDebugStreamLine("LauncherDown_Helper) #3 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);

	motor[Loader1] = 0;

	powerLoader = 80;
	motor[Loader1] = powerLoader;

	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 70);
	motor[Loader1] = 0;

	originalPower = launcherPower;
	primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	unsigned long cTime = nPgmTime + 300;
	waitUntil(cTime <= nPgmTime);

	waitUntil(SensorValue[Launcher1_Ready] == 1);

	originalPower = 15;
	primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	//waitUntil(SensorValue[Launcher1_Loaded] == 0);
	//writeDebugStreamLine("LauncherDown_Helper) #4 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);
	//startTask(LauncherStop);


}



void LauncherDown_Helper_Old()
{// T3


	// Turn right to the center
	nMotorEncoder(Launcher1_L) = 0;

	int originalPower = 100;
	int primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	writeDebugStreamLine("LauncherDown_Helper) #1 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));

	waitUntil(abs(nMotorEncoder(Launcher1_L)) >= 200);

	writeDebugStreamLine("LauncherDown_Helper) #2 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));


	originalPower = 15;
	primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;


	nMotorEncoder(Loader1) = 0;
	int powerLoader = -45;
	motor[Loader1] = powerLoader;

	waitUntil(abs(nMotorEncoder(Loader1)) >= 90 || SensorValue[Launcher1_BallLoaded] == 1);

	writeDebugStreamLine("LauncherDown_Helper) 00 Launcher1_Loaded :%d", SensorValue[Launcher1_BallLoaded]);
	writeDebugStreamLine("LauncherDown_Helper) 00 Loader1 :%d", abs(nMotorEncoder(Loader1)));

	motor[Loader1] = 0;

	clearTimer(T3);
	waitUntil(SensorValue[Launcher1_BallLoaded] == 1 || time1[T3] > 1000);

	if (time1[T3] > 1000)
	{
		startTask(LauncherStop);
		return;
	}

	powerLoader = 55;
	motor[Loader1] = powerLoader;


	nMotorEncoder(Loader1) = 0;
	waitUntil(abs(nMotorEncoder(Loader1)) >= 20);
	motor[Loader1] = 0;


	//wait1Msec(10);

	//powerLoader = 80;
	//motor[Loader1] = powerLoader;
	//wait1Msec(100);

	//motor[Loader1] = 0;

	//writeDebugStreamLine("LauncherDown_Helper) 01 Launcher1_Loaded :%d", SensorValue[Launcher1_Loaded]);




	//wait1Msec(100);

	//motor[Loader1] = 0;
	//wait1Msec(50);

	//powerLoader = 20;
	//motor[Loader1] = powerLoader;
	//wait1Msec(80);


	//motor[Loader1] = 0;


	wait1Msec(200);
	originalPower = 100;
	primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;


	writeDebugStreamLine("LauncherDown_Helper) #3 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));

	waitUntil(SensorValue[Launcher1_BallLoaded] == 0 || abs(nMotorEncoder(Launcher1_L)) >= 509);

	writeDebugStreamLine("LauncherDown_Helper) #4 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));

	//motor[Loader1] = 0;

	startTask(LauncherStop);

	wait1Msec(10);
	originalPower = -30;
	primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	waitUntil(abs(nMotorEncoder(Launcher1_L)) < 522);
	writeDebugStreamLine("LauncherDown_Helper) #5 launcherPosition :%d", abs(nMotorEncoder(Launcher1_L)));

	startTask(LauncherStop);


}

task LauncherUp()
{// T3
	//stopTask(LauncherStop);

	int originalPower = -110;
	int primaryPower = originalPower;

	motor[Launcher1_H] = primaryPower;
	motor[Launcher1_M] = primaryPower;
	motor[Launcher1_L] = primaryPower;

	wait1Msec(70);

	//originalPower =  -19;
	//primaryPower = originalPower;

	//motor[Launcher1] = primaryPower;
	//motor[Launcher2] = primaryPower;
	//motor[Launcher3] = primaryPower;


	startTask(LauncherStop);
}

task LauncherStop()
{
	LauncherStop_Helper();
}

void LauncherStop_Helper()
{
	int power = 0;

	motor[Launcher1_H] = power;
	motor[Launcher1_M] = power;
	motor[Launcher1_L] = power;
}
