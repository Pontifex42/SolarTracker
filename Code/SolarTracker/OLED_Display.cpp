#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>
#include "PinConfig.h"
#include "OLED_Display.h"

// #define DEBUG_OLED

#ifndef DEBUG_OLED

#ifdef DEBUG_PRINTLN
#undef DEBUG_PRINTLN
#define DEBUG_PRINTLN(a)
#undef DEBUG_PRINT
#define DEBUG_PRINT(a)
#endif

#endif


#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SH1106G display = Adafruit_SH1106G(DISPLAY_WIDTH, DISPLAY_HIGHT, &Wire);

#define FSM_LOGO_WIDTH 128
#define FSM_LOGO_HEIGHT 64

const unsigned char gImage_FSM_Logo[1024] = {
0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0X80,0X01,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XC0,0X07,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XE0,0X07,0X9E,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0XE0,0X0E,0X07,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0X70,0X0C,0X03,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XCE,0X30,0X1C,0XF3,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XDF,0X30,0X1C,0XF3,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0X9F,0X30,0X1C,0XF3,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XDF,0X30,0X1C,0XF3,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XCE,0X70,0X0C,0X63,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0X70,0X0E,0X07,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XF0,0XE0,0X07,0X1E,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XE0,0X07,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X80,0X03,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0X80,0X01,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X80,0X03,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XDF,0XFF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XFF,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0X81,0XC0,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X1F,0XF0,0X0F,0XF0,0X07,0XFC,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X7F,0X80,0X1F,0XF8,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XFE,0X7C,0X3F,0XFC,0X30,0X3F,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0XFC,0X00,0X07,0XF1,0XFE,0X7F,0XFC,0X78,0X0F,0XE0,0X00,0X08,0X00,0X00,
0X00,0X01,0XFF,0X00,0X0F,0XC7,0XFE,0X7F,0XFC,0X7C,0X73,0XF0,0X00,0XFE,0X00,0X00,
0X00,0X01,0XFF,0X80,0X1F,0X9F,0XFE,0XFE,0X7C,0XFC,0XF8,0XFC,0X03,0XFF,0X00,0X00,
0X00,0X03,0XDF,0XE0,0X7E,0X3F,0XFE,0XFC,0X3C,0XFC,0XF8,0X7E,0X07,0XFF,0X00,0X00,
0X00,0X03,0X83,0XF0,0XFC,0X3F,0X8E,0XFC,0X3C,0XFD,0XFC,0X1F,0X1F,0XC7,0X80,0X00,
0X7C,0X03,0X80,0XFD,0XF0,0X3F,0X86,0X7E,0X1D,0XFF,0XFC,0X0F,0XBF,0X07,0X80,0X00,
0XFF,0X03,0X80,0X7F,0XE0,0X3F,0X80,0X7F,0X81,0XFF,0XFE,0X07,0XFC,0X07,0X00,0XFE,
0XFF,0XC3,0X80,0X1F,0XC0,0X3F,0XF8,0X7F,0XC1,0XFF,0XFE,0X03,0XF8,0X07,0X03,0XFF,
0XFF,0XE3,0XC0,0X0F,0X80,0X1F,0XF8,0X3F,0XF1,0XFF,0XBE,0X01,0XF0,0X07,0X0F,0XFE,
0X03,0XFB,0XC0,0X07,0XC0,0X1F,0XF8,0X1F,0XF1,0XF7,0XBE,0X03,0XE0,0X07,0X1F,0XC0,
0X00,0XFF,0X80,0X03,0XF0,0X1F,0XF8,0X07,0XF1,0XF3,0X1E,0X0F,0XC0,0X07,0XFF,0X00,
0X00,0X3F,0X80,0X00,0XF8,0X1F,0XB8,0X03,0XF9,0XF0,0X1E,0X1F,0X00,0X07,0XFC,0X00,
0X00,0X1F,0X00,0X07,0XFC,0X1F,0X81,0X81,0XF9,0XE0,0X1E,0X3E,0X00,0X03,0XF0,0X00,
0X00,0X00,0X00,0X1F,0XFF,0X1F,0X81,0XE3,0XF9,0XE0,0X1E,0XFF,0XE0,0X00,0XC0,0X00,
0X00,0X00,0X00,0X3F,0XFF,0X9F,0X81,0XFF,0XF1,0XE0,0X01,0XFF,0XF0,0X00,0X00,0X00,
0X00,0X00,0X00,0X7E,0X0F,0XE7,0X01,0XFF,0XF1,0XE0,0X07,0XFF,0XF8,0X00,0X00,0X00,
0X00,0X00,0X00,0X78,0X03,0XF8,0X00,0XFF,0XE1,0XE0,0X1F,0XC0,0X7C,0X00,0X00,0X00,
0X00,0X00,0X00,0XF0,0X00,0XFE,0X00,0XFF,0XE1,0XE0,0X7F,0X00,0X3C,0X00,0X00,0X00,
0X00,0X00,0X00,0XE0,0X00,0X7F,0XC0,0X7F,0X80,0XC3,0XFE,0X00,0X1C,0X00,0X00,0X00,
0X00,0X00,0X00,0XE0,0X00,0XFF,0XF8,0X3F,0X00,0X1F,0XFC,0X00,0X1E,0X00,0X00,0X00,
0X00,0X00,0X00,0XE0,0X01,0XFF,0XFF,0X80,0X01,0XFF,0XFF,0X00,0X0E,0X00,0X00,0X00,
0X00,0X00,0X01,0XE0,0X03,0XE0,0XFF,0XFF,0XFF,0XFF,0XBF,0X80,0X0E,0X00,0X00,0X00,
0X00,0X00,0X01,0XE0,0X03,0XC0,0X1F,0XFF,0XFF,0XFC,0X07,0XC0,0X0E,0X00,0X00,0X00,
0X00,0X00,0X01,0XC0,0X07,0X80,0X03,0XFF,0XFF,0XC0,0X03,0XC0,0X0E,0X00,0X00,0X00,
0X00,0X00,0X01,0XC0,0X07,0X80,0X00,0X07,0XF0,0X00,0X01,0XC0,0X0F,0X00,0X00,0X00,
0X00,0X7E,0X03,0XC0,0X07,0X00,0X00,0X00,0X00,0X00,0X01,0XC0,0X07,0X00,0XF0,0X00,
0X00,0XFF,0XFF,0XC0,0X07,0X00,0X00,0X00,0X00,0X00,0X01,0XC0,0X07,0X87,0XFC,0X00,
0X00,0XFF,0XFF,0X80,0X07,0X80,0X00,0X00,0X00,0X00,0X03,0XC0,0X03,0XFF,0XFE,0X00,
0X01,0XE7,0XFF,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0XC0,0X03,0XFF,0XFE,0X00,
0X01,0XC0,0XFC,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X80,0X01,0XFE,0X0F,0X00,
0X01,0X80,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X70,0X07,0X00,
0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X07,0X80,0X00,0X00,0X02,0X00,
0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X07,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0X80,0X00,0X00,0X00,0X00,0X07,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XFF,0X80,0X00,0X00,0X00,0X00,0X03,0XC0,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X07,0XFF,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0XFE,0X00,0X00,0X00,0X00,0X00,0X01,0XFF,0XE0,0X00,0X00,0X00,
0X00,0X00,0X00,0X0F,0XF8,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XE0,0X00,0X00,0X00,
0X00,0X00,0X00,0X0E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X1F,0XE0,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X00,0X00,
};

void SetupOled()
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SCREEN_ADDRESS))
	{
		DEBUG_PRINTLN("SSD1306 allocation failed");
		return;
	}

	display.clearDisplay();

	// let there be an appearance of the flying spaghetti monsters to the unbelievers!
	display.drawBitmap(
		(display.width() - FSM_LOGO_WIDTH) / 2,
		(display.height() - FSM_LOGO_HEIGHT) / 2,
		gImage_FSM_Logo, FSM_LOGO_WIDTH, FSM_LOGO_HEIGHT, 1);

	display.display();

	// Clear the buffer
	display.clearDisplay();
	display.setTextSize(1);      // Normal 1:1 pixel scale
	display.setTextColor(SH110X_WHITE); // Draw white text // display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
	display.setCursor(0, 0);     // Start at top-left corner
	display.cp437(true);         // Use full 256 char 'Code Page 437' font
	// delay(700); // let the fsm logo be visible
}

int curCol = SH110X_BLACK;

void OledMessage(double solar_panel_voltage)
{
	// Solar panel voltage message on the OLED screen.
	display.clearDisplay();
	display.setTextSize(2);
	display.setCursor(7, 0);
	display.print("Solar Volt");
	display.setTextSize(3);
	display.setCursor(20, 30);
	display.print(solar_panel_voltage, 2);
	display.display();
}
