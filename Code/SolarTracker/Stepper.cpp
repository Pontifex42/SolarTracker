#include "Arduino.h"
#include "PinConfig.h"
#include "Stepper.h"
#include "LimitSwitch.h"

// #define DEBUG_STEPPER

#ifndef DEBUG_STEPPER
#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif
#endif

// depending on speed: Highest speed 10 leads to 1200 microseconds, which might be way too fast for the motors
// every speed level more increases the value by factor 1.4
//  10	   9     8    7     6     5     4      3      2     1
// 1500, 2100, 2940, 4116, 5762, 8067, 11294, 15812, 22136, 30991
#define FULL_SPEED_TIMING 1500

int MicroSecPerStep = 3292; // usual limit is 2-3ms per step, regarding to datasheet. Mine work still at 1,5ms.

void SetStepperTiming(int speed)
{
	if (speed < 1)
		speed = 1;
	else if (speed > 10)
		speed = 10;

	int inverseLevel = 10 - speed;
	double factor = pow(1.4, inverseLevel);
	MicroSecPerStep = FULL_SPEED_TIMING * factor;
}

unsigned long lastStepTime_lr = 0;
unsigned long lastStepTime_ud = 0;

void ExecuteStep_lr(int direction)
{
	while ((micros() - lastStepTime_lr) < MicroSecPerStep)
		;

	if (GetLimitSwitchL() && (direction == DIR_LEFT))
		return;

	if (GetLimitSwitchR() && (direction == DIR_RIGHT))
		return;

	// compute the next phase number
	static int stepPhase_l = 0;

	stepPhase_l += direction;

	if (stepPhase_l <= -1)
		stepPhase_l = 3;

	if (stepPhase_l >= 4)
		stepPhase_l = 0;

	// set the coils for this phase
	switch (stepPhase_l)
	{
	case 3:
		digitalWrite(PIN_STEPPER_LR_IN1, LOW);
		digitalWrite(PIN_STEPPER_LR_IN2, LOW);
		digitalWrite(PIN_STEPPER_LR_IN3, HIGH);
		digitalWrite(PIN_STEPPER_LR_IN4, HIGH);
		break;
	case 2:
		digitalWrite(PIN_STEPPER_LR_IN1, LOW);
		digitalWrite(PIN_STEPPER_LR_IN2, HIGH);
		digitalWrite(PIN_STEPPER_LR_IN3, HIGH);
		digitalWrite(PIN_STEPPER_LR_IN4, LOW);
		break;
	case 1:
		digitalWrite(PIN_STEPPER_LR_IN1, HIGH);
		digitalWrite(PIN_STEPPER_LR_IN2, HIGH);
		digitalWrite(PIN_STEPPER_LR_IN3, LOW);
		digitalWrite(PIN_STEPPER_LR_IN4, LOW);
		break;
	case 0:
		digitalWrite(PIN_STEPPER_LR_IN1, HIGH);
		digitalWrite(PIN_STEPPER_LR_IN2, LOW);
		digitalWrite(PIN_STEPPER_LR_IN3, LOW);
		digitalWrite(PIN_STEPPER_LR_IN4, HIGH);
		break;
	}
	lastStepTime_lr = micros();
}

void ExecuteStep_ud(int direction)
{
	static int stepPhase_r = 0;

	// compute the next phase number
	stepPhase_r += direction;

	if (stepPhase_r <= -1)
		stepPhase_r = 3;

	if (stepPhase_r >= 4)
		stepPhase_r = 0;

	while ((micros() - lastStepTime_ud) < MicroSecPerStep)
		;

	// set the coils for this phase
	switch (stepPhase_r)
	{
	case 0:
		digitalWrite(PIN_STEPPER_UD_IN1, LOW);
		digitalWrite(PIN_STEPPER_UD_IN2, LOW);
		digitalWrite(PIN_STEPPER_UD_IN3, HIGH);
		digitalWrite(PIN_STEPPER_UD_IN4, HIGH);
		break;
	case 1:
		digitalWrite(PIN_STEPPER_UD_IN1, LOW);
		digitalWrite(PIN_STEPPER_UD_IN2, HIGH);
		digitalWrite(PIN_STEPPER_UD_IN3, HIGH);
		digitalWrite(PIN_STEPPER_UD_IN4, LOW);
		break;
	case 2:
		digitalWrite(PIN_STEPPER_UD_IN1, HIGH);
		digitalWrite(PIN_STEPPER_UD_IN2, HIGH);
		digitalWrite(PIN_STEPPER_UD_IN3, LOW);
		digitalWrite(PIN_STEPPER_UD_IN4, LOW);
		break;
	case 3:
		digitalWrite(PIN_STEPPER_UD_IN1, HIGH);
		digitalWrite(PIN_STEPPER_UD_IN2, LOW);
		digitalWrite(PIN_STEPPER_UD_IN3, LOW);
		digitalWrite(PIN_STEPPER_UD_IN4, HIGH);
		break;
	}

	lastStepTime_ud = micros();
}

void MoveBothSteppers(int lr, int ud)
{
	long steps_abs_lr = abs(lr);
	long steps_abs_ud = abs(ud);
	long over = 0;

	if (steps_abs_lr > steps_abs_ud)
	{
		for (long i = 0; i < steps_abs_lr; ++i)
		{
			ExecuteStep_lr(lr > 0 ? DIR_LEFT : DIR_RIGHT);
			over += steps_abs_ud;
			if (over >= steps_abs_lr)
			{
				over -= steps_abs_lr;
				ExecuteStep_ud(ud > 0 ? DIR_UP : DIR_DOWN);
			}
		}
	}
	else
	{
		for (long i = 0; i < steps_abs_ud; ++i)
		{
			ExecuteStep_ud(ud > 0 ? DIR_UP : DIR_DOWN);
			over += steps_abs_lr;
			if (over >= steps_abs_ud)
			{
				over -= steps_abs_ud;
				ExecuteStep_lr(lr > 0 ? DIR_LEFT : DIR_RIGHT);
			}
		}
	}
}

void DeactivateStepper()
{
	while ((micros() - lastStepTime_lr) < MicroSecPerStep)
		;// Let motors finish their current step

	while ((micros() - lastStepTime_ud) < MicroSecPerStep)
		;
	// Let motors finish their current step

	digitalWrite(PIN_STEPPER_LR_IN1, LOW);
	digitalWrite(PIN_STEPPER_LR_IN2, LOW);
	digitalWrite(PIN_STEPPER_LR_IN3, LOW);
	digitalWrite(PIN_STEPPER_LR_IN4, LOW);

	digitalWrite(PIN_STEPPER_UD_IN1, LOW);
	digitalWrite(PIN_STEPPER_UD_IN2, LOW);
	digitalWrite(PIN_STEPPER_UD_IN3, LOW);
	digitalWrite(PIN_STEPPER_UD_IN4, LOW);
}

void DeactivateStepperLR()
{
	while ((micros() - lastStepTime_lr) < MicroSecPerStep)
		;// Let motors finish their current step

	// Let motors finish their current step
	digitalWrite(PIN_STEPPER_LR_IN1, LOW);
	digitalWrite(PIN_STEPPER_LR_IN2, LOW);
	digitalWrite(PIN_STEPPER_LR_IN3, LOW);
	digitalWrite(PIN_STEPPER_LR_IN4, LOW);
}

void DeactivateStepperUD()
{
	while ((micros() - lastStepTime_ud) < MicroSecPerStep)
		;
	// Let motors finish their current step

	digitalWrite(PIN_STEPPER_UD_IN1, LOW);
	digitalWrite(PIN_STEPPER_UD_IN2, LOW);
	digitalWrite(PIN_STEPPER_UD_IN3, LOW);
	digitalWrite(PIN_STEPPER_UD_IN4, LOW);
}



void ReactivateStepper()
{
	ExecuteStep_lr(0);
	ExecuteStep_ud(0);
}

const int LimitToLimitSteps = 1096;
const int UpToDownSteps = 500;

void InitializeStepperPosition()
{
	DEBUG_PRINTLN("InitializeStepperPosition");
	ReactivateStepper();
	
	while (!GetLimitSwitchL()) // Rotate to left switch
	{
		MoveBothSteppers(DIR_LEFT, 0);
		delay(5);
	}
	DEBUG_PRINTLN("Left limitreached");

	MoveBothSteppers(0, DIR_DOWN * UpToDownSteps); // Press panel against limit
	DEBUG_PRINTLN("Down to limit");

	// Move both steppers half the way back
	MoveBothSteppers(DIR_RIGHT * (LimitToLimitSteps / 2), DIR_UP * (UpToDownSteps / 2));
	DEBUG_PRINTLN("Stepper initialized");
	DeactivateStepper();
}


void SetupStepper()
{
	DEBUG_PRINTLN("SetupStepper");

	lastStepTime_lr =
	lastStepTime_ud = micros();
	SetStepperTiming(7);

	pinMode(PIN_STEPPER_LR_IN1, OUTPUT);
	pinMode(PIN_STEPPER_LR_IN2, OUTPUT);
	pinMode(PIN_STEPPER_LR_IN3, OUTPUT);
	pinMode(PIN_STEPPER_LR_IN4, OUTPUT);

	pinMode(PIN_STEPPER_UD_IN1, OUTPUT);
	pinMode(PIN_STEPPER_UD_IN2, OUTPUT);
	pinMode(PIN_STEPPER_UD_IN3, OUTPUT);
	pinMode(PIN_STEPPER_UD_IN4, OUTPUT);
	
	DeactivateStepper();
}
