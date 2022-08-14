/*
 Name:		SolarTracker.ino
 Created:	13.08.2022 12:35:16
 Author:	Thorsten
*/
#include "PinConfig.h"
#include <WiFi.h>
#include "LimitSwitch.h"
#include "Stepper.h"
#include "OLED_Display.h"
#include "LDR.h"
#include "SolarPanel.h"

unsigned long timeLastDisplayed;
#define MILLIS_DISPLAY_DELAY 500
void setup() 
{
	setCpuFrequencyMhz(240);
	WiFi.mode(WIFI_OFF);
	WiFi.setSleep(true);

	Serial.begin(115200);

	SetupLimitSwitch();

	SetupStepper();
	SetupOled();
	SetupLDR();
	SetupSolarPanel();

	InitializeStepperPosition();
	timeLastDisplayed = millis();
	DEBUG_PRINTLN("Setup OK");
}

const int tolerance = 60;

void loop() 
{
	int up, left;
	ReadLDR(up, left);
	
//	DEBUG_PRINTLN("Avg UD:" + String(up) + " Avg LR:" + String(left));
	if (abs(up) > tolerance)
	{
		if (up > 0)
			ExecuteStep_ud(DIR_DOWN);
		else if (up < 0)
			ExecuteStep_ud(DIR_UP);
	}
	else
		DeactivateStepperUD();

	if (abs(left) > tolerance)
	{
		if (left > 0)
			ExecuteStep_lr(DIR_RIGHT);
		else if (left < 0)
			ExecuteStep_lr(DIR_LEFT);
	}
	else
		DeactivateStepperLR();


	if ((millis() - timeLastDisplayed) > MILLIS_DISPLAY_DELAY)
	{
		double volt = ReadSolarPanel();
		OledMessage(volt);
		timeLastDisplayed = millis();
	}

	// for this may react a bit too fast sometimes, one may add e delay here
}
