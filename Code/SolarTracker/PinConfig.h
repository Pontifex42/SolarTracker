#pragma once
#include "Arduino.h"

// Limit switches
#define PIN_LIMITSWITCH_L	2 // Remove stupid blue LED and 1k Resistor before using GPIO2 as input!!
#define PIN_LIMITSWITCH_R	4

// Stepper latitude (right/left)
#define PIN_STEPPER_LR_IN1	26
#define PIN_STEPPER_LR_IN2	25
#define PIN_STEPPER_LR_IN3	17
#define PIN_STEPPER_LR_IN4	16

// Stepper altitude (up/down)
#define PIN_STEPPER_UD_IN1	27
#define PIN_STEPPER_UD_IN2	14
#define PIN_STEPPER_UD_IN3	12
#define PIN_STEPPER_UD_IN4	13

// Display 1302
#define PIN_I2C_SDA			21
#define PIN_I2C_SCL			22

// Alternative: TFT display via SPI
#define PIN_TFT_SCLK		18
#define PIN_TFT_MOSI		23
#define PIN_TFT_RST			15
#define PIN_TFT_DATA_CMD	19
#define PIN_TFT_CS			5
#define PIN_TFT_LED			32

// LDR-Sensors
#define PIN_LDR_UL			39
#define PIN_LDR_UR			36
#define PIN_LDR_DL			35
#define PIN_LDR_DR			34

// Solar panel voltage
#define PIN_SOLAR_MESASURE	33


#define DEBUG_PRINTLN(a) Serial.println(a)
//#define DEBUG_PRINTLN(a)
#define DEBUG_PRINT(a) Serial.print(a)
//#define DEBUG_PRINT(a)
