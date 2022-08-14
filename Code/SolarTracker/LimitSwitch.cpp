#include "PinConfig.h"
#include "LimitSwitch.h"

esp_timer_handle_t timerLimitSwitch; // for debouncing of switches
#define DEBOUNCE_BUTTON 3000 // 3ms should do well for usual switch quality

bool stateSW_L, stateSW_R;

bool GetLimitSwitchL()
{
	return stateSW_L; // no semaphore/critical section here. Bool should be atomar to read and write.
}

bool GetLimitSwitchR()
{
	return stateSW_R; // no semaphore/critical section here. Bool should be atomar to read and write.
}

void ISR_OnSW_L()
{
	esp_timer_stop(timerLimitSwitch);
	esp_timer_start_once(timerLimitSwitch, DEBOUNCE_BUTTON);
}

void ISR_OnSW_R()
{
	esp_timer_stop(timerLimitSwitch);
	esp_timer_start_once(timerLimitSwitch, DEBOUNCE_BUTTON);
}

void OnTimerSwitch(void* arg)
{
	stateSW_L = (digitalRead(PIN_LIMITSWITCH_L) == 0);
	stateSW_R = (digitalRead(PIN_LIMITSWITCH_R) == 0);
	DEBUG_PRINTLN("L:" + String(stateSW_L) + "  R:" + String(stateSW_R));
}

void SetupLimitSwitch()
{
	pinMode(PIN_LIMITSWITCH_L, INPUT_PULLUP);
	pinMode(PIN_LIMITSWITCH_R, INPUT_PULLUP);

	esp_timer_create_args_t timerConfigLimitSwitch;
	timerConfigLimitSwitch.arg = NULL;
	timerConfigLimitSwitch.callback = OnTimerSwitch;
	timerConfigLimitSwitch.dispatch_method = ESP_TIMER_TASK;
	timerConfigLimitSwitch.name = "LimitSwitch";

	if (esp_timer_create(&timerConfigLimitSwitch, &timerLimitSwitch) != ESP_OK)
	{
		DEBUG_PRINTLN("timerButton create failed.");
	}

	stateSW_L = (digitalRead(PIN_LIMITSWITCH_L) == 0); // deactivated switch returns high, activated switch returns low
	stateSW_R = (digitalRead(PIN_LIMITSWITCH_R) == 0);

	attachInterrupt(digitalPinToInterrupt(PIN_LIMITSWITCH_L), ISR_OnSW_L, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_LIMITSWITCH_R), ISR_OnSW_R, CHANGE);
}
