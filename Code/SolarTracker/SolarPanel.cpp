#include "PinConfig.h"
#include "SolarPanel.h"

void SetupSolarPanel()
{
	pinMode(PIN_SOLAR_MESASURE, INPUT);
}

const double ref = 260.0; // measured, does not work very well
double ReadSolarPanel()
{
	uint16_t val = analogRead(PIN_SOLAR_MESASURE);
	//DEBUG_PRINTLN(val);
	double voltage = val / ref;
	voltage += 0.45; // offset measured
	return voltage;
}