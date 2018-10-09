/*
 NewhavenLCDSerial.h - Library for controlling a Newhaven Serial LCD module with RS-232 via Software Serial

 Based on serLCD by version 1.5 by Jordan Hochenbaum and Cody B. Null


*/
#ifndef NewhavenLCDSerial_h
#define NewhavenLCDSerial_h

#include "SoftwareSerial.h"
#include <Print.h>
#if ARDUINO >= 100
#include "Arduino.h"       // for delayMicroseconds,digitalPinToBitMask, etc
#else
#include "WProgram.h"      // for delayMicroseconds
#include "pins_arduino.h"  // for digitalPinToBitMask, etc
#endif

#define LCD_DELAY_SHORT 	150
#define LCD_DELAY_LONG  	1550
#define LCD_DELAY_VERYLONG	11

#define LCD_CMD_PREFIX		0xFE
// Commands
#define LCD_DISPLAYON		0x41
#define LCD_DISPLAYOFF	 	0x42	
#define LCD_SETCURSOR		0x45
#define LCD_HOME		0x46
#define LCD_UNDERLINEON		0x47
#define LCD_UNDERLINEOFF	0x48
#define LCD_MOVECURSORLEFT	0x49
#define LCD_MOVECURSORRIGHT	0x4A
#define LCD_BLINKON		0x4B
#define LCD_BLINKOFF		0x4C
#define LCD_BACKSPACE		0x4E
#define LCD_CLEAR		0x51
#define LCD_CONTRAST		0x52
#define LCD_BRIGHTNESS		0x53
#define LCD_LOADCUSTOMCHAR	0x54
#define LCD_MOVELEFT		0x55
#define LCD_MOVERIGHT		0x56
#define LCD_SETBAUDRATE		0x61
#define LCD_SETI2CADDR		0x62
#define LCD_DISPLAYFIRMWARE	0x70
#define LCD_DISPLAYBAUDRATE	0x71
#define LCD_DISPLAYI2CADDR	0x72


// Flags for display size
#define LCD_2LINE		0x02
#define LCD_4LINE		0x04
#define LCD_16CHAR		0x10
#define LCD_20CHAR		0x14

//  Flags for setting display size
#define LCD_SET2LINE		0x06
#define LCD_SET4LINE		0x05
#define LCD_SET16CHAR		0x04
#define LCD_SET20CHAR		0x03

class NewhavenLCDSerial : public SoftwareSerial {

public:
	NewhavenLCDSerial (int pin);

	void clear();
//	void clearLine(int);
	void home();
	void setBrightness(int);
	void setContrast(int);
//	void setSplash();
//	void toggleSplash();

	void blink();
	void noBlink();
	void cursor();
	void noCursor();
	void display();
	void noDisplay();

	void setCursor(int, int);
//	void selectLine(int);

	void setBaudRate(int);
	void backspace();
	void moveCursorLeft();
	void moveCursorRight();
	void moveLeft();
	void moveRight();
	void displayBaudRate();
	void displayI2CAddress();
//	void createChar(int, uint8_t[]);
//	void printCustomChar(int);

private:
	void commandShort(uint8_t);
	void commandLong(uint8_t);
	void commandVeryLong(uint8_t);

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _numchars;
	uint8_t _rowoffset;
};

#endif