#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB
#define M_SIZE 1.3333


// float ltx = 0;    // Saved x coord of bottom of needle
// uint16_t osx = M_SIZE*120, osy = M_SIZE*120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update

// int old_analog =  -999; // Value last displayed

// int value[6] = {0, 0, 0, 0, 0, 0};
// int old_value[6] = { -1, -1, -1, -1, -1, -1};
// int d = 0;

int speed = 0;
String gear = "N";

class SHCustomProtocol {
private:

public:

	/*
	CUSTOM PROTOCOL CLASS
	SEE https://github.com/zegreatclan/SimHub/wiki/Custom-Arduino-hardware-support

	GENERAL RULES :
		- ALWAYS BACKUP THIS FILE, reinstalling/updating SimHub would overwrite it with the default version.
		- Read data AS FAST AS POSSIBLE in the read function
		- NEVER block the arduino (using delay for instance)
		- Make sure the data read in "read()" function READS ALL THE DATA from the serial port matching the custom protocol definition
		- Idle function is called hundreds of times per second, never use it for slow code, arduino performances would fall
		- If you use library suspending interrupts make sure to use it only in the "read" function when ALL data has been read from the serial port.
			It is the only interrupt safe place

	COMMON FUNCTIONS :
		- FlowSerialReadStringUntil('\n')
			Read the incoming data up to the end (\n) won't be included
		- FlowSerialReadStringUntil(';')
			Read the incoming data up to the separator (;) separator won't be included
		- FlowSerialDebugPrintLn(string)
			Send a debug message to simhub which will display in the log panel and log file (only use it when debugging, it would slow down arduino in run conditions)

	*/


	// Called when starting the arduino (setup method in main sketch)
	void setup() {
		tft.init();
  		tft.setRotation(3);
  		tft.fillScreen(TFT_BLACK);

  		updateTime = millis(); // Next update time
	}

	// Called when new data is coming from computer
	void read() {
		speed = FlowSerialReadStringUntil(';').toInt();
		gear  = FlowSerialReadStringUntil(';');
	}

	// Called once per arduino loop, timing can't be predicted, 
	// but it's called between each command sent to the arduino
	void loop() {
		if (updateTime <= millis()) {
		//   tft.setCursor(20, 0);
		//   tft.setTextFont(2);
		//   tft.setTextSize(2);
		//   tft.setTextColor(TFT_WHITE, TFT_BLACK);
		//   tft.println("Test line 1");
		//   tft.println("test line 2");
		  // top line

		  tft.drawRect(0,75,480,1,TFT_WHITE);
		  tft.drawRect(75,0,1,75,TFT_WHITE);
		  tft.drawRect(240,0,1,75,TFT_WHITE);
		  tft.drawRect(405,0,1,75,TFT_WHITE);

		  tft.drawRect(160,75,1,320-75,TFT_WHITE);
		  tft.drawRect(480-160,75,1,320-75,TFT_WHITE);

	      tft.drawRect(0,150,160,1, TFT_WHITE);
		  tft.drawRect(480-160,150,160,1, TFT_WHITE);

		  tft.drawRect(0,320 - 75,480, 1, TFT_WHITE);

		  tft.setCursor(0,55,2);
		  tft.setTextSize(1);
		  tft.setTextColor(TFT_WHITE, TFT_BLACK);
		  tft.print("SPEED");


		// 	FlowSerialDebugPrintLn("LOOP");
    	  updateTime = millis() + 35; // Update emter every 35 milliseconds
 
    		// Create a Sine wave for testing
    		// d += 4; if (d >= 360) d = 0;
    		// value[0] = 50 + 50 * sin((d + 0) * 0.0174532925);
 
    		// plotNeedle(value[0], 0); // It takes between 2 and 12ms to replot the needle with zero delay
  		}
	}

	// Called once between each byte read on arduino,
	// THIS IS A CRITICAL PATH :
	// AVOID ANY TIME CONSUMING ROUTINES !!!
	// PREFER READ OR LOOP METHOS AS MUCH AS POSSIBLE
	// AVOID ANY INTERRUPTS DISABLE (serial data would be lost!!!)
	void idle() {
	}

};


#endif