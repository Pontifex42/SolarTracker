#pragma once
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>

#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HIGHT 64 // OLED display height, in pixels

void SetupOled();

void OledMessage(double solar_panel_voltage);
