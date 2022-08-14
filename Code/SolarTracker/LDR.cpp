#include "PinConfig.h"
#include "LDR.h"

// #define DEBUG_LDR

#ifndef DEBUG_LDR
#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif
#endif

void SetupLDR()
{
    pinMode(PIN_LDR_UL, ANALOG);
    pinMode(PIN_LDR_UR, ANALOG);
    pinMode(PIN_LDR_DL, ANALOG);
    pinMode(PIN_LDR_DR, ANALOG);
}


void ReadLDR(int &updown, int &leftright)
{
    int UL = analogRead(PIN_LDR_UL);
    int UR = analogRead(PIN_LDR_UR);
    int DL = analogRead(PIN_LDR_DL);
    int DR = analogRead(PIN_LDR_DR);

    // DEBUG_PRINTLN("UL:" + String(UL) + " UR:" + String(UR) + " DL:" + String(DL) + " DR:" + String(DR));
    int AverageUp = (UL + UR) / 2;                                   // Calculation of the average voltage value of the LDRs on each side.
    int AverageDown = (DL + DR) / 2;
    int AverageRight = (UR + DR) / 2;
    int AverageLeft = (UL + DL) / 2;

    updown = AverageUp - AverageDown;
    leftright = AverageLeft - AverageRight;
}
