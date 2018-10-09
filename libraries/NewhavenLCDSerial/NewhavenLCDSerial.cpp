/*
  NewhavenLCDSerial.cpp - Library for controlling a Newhaven Serial LCD module with RS-232 via Software Serial

 Based on serLCD by version 1.5 by Jordan Hochenbaum and Cody B. Null

*/

#include "NewhavenLCDSerial.h"

//	PUBLIC FUNCTIONS

// Contstructor
// defaults to 16x2 display
NewhavenLCDSerial::NewhavenLCDSerial(int pin) : SoftwareSerial(pin, pin){
	pinMode(pin, OUTPUT);
	begin(9600);
	_numlines = LCD_2LINE;
	_numchars = LCD_16CHAR;
	_rowoffset = 0;
}

// Set backlight brightness. Value range 1-8. Default 1.
void NewhavenLCDSerial::setBrightness(int val){
	if(val >= 1 && val <= 8){
	  write(LCD_CMD_PREFIX);
	 write(LCD_BRIGHTNESS);
 	 write(val);
	 delayMicroseconds(LCD_DELAY_SHORT);
	}
}

// Set display Contrast. Value range 1-50. Default 40.
void NewhavenLCDSerial::setContrast(int val){
	if(val >= 1 && val <= 50){
	  write(LCD_CMD_PREFIX);
	  write(LCD_CONTRAST);
 	  write(val);
	  delayMicroseconds(LCD_DELAY_LONG);
	}
}

// Clears screen and returns cursor to home position
void NewhavenLCDSerial::clear(){
	commandLong(LCD_CLEAR);
}

// Clears a single line by writing blank spaces then returning
// cursor to beginning of line
/*void NewhavenLCDSerial::clearLine(int num){
	if(num > 0 && num <= _numlines){
		setCursor(num, 1);
		print("                ");
		setCursor(num, 1);
	}
}
// Moves cursor to the beginning of selected line
void NewhavenLCDSerial::selectLine(int num){
	if(num > 0 && num <= _numlines){
		setCursor(num, 1);
	}
}
*/

// returns cursor to home position
void NewhavenLCDSerial::home(){
	commandLong(LCD_HOME);
}



// Blinking cursor on/off
void NewhavenLCDSerial::blink(){
	commandShort(LCD_BLINKON);
}
void NewhavenLCDSerial::noBlink(){
	commandShort(LCD_BLINKOFF);
}

// Underline cursor on/off
void NewhavenLCDSerial::cursor(){
	commandLong(LCD_UNDERLINEON);
}
void NewhavenLCDSerial::noCursor(){
	commandLong(LCD_UNDERLINEOFF);
}

// Display on/off
void NewhavenLCDSerial::display(){
	commandShort(LCD_DISPLAYON);
}
void NewhavenLCDSerial::noDisplay(){
	commandShort(LCD_DISPLAYOFF);
}

// Move Cursor
void NewhavenLCDSerial::moveCursorLeft(){
	commandShort(LCD_MOVECURSORLEFT);
}
void NewhavenLCDSerial::moveCursorRight(){
	commandShort(LCD_MOVECURSORRIGHT);
}

// Move Display
void NewhavenLCDSerial::moveLeft(){
	commandShort(LCD_MOVELEFT);
}
void NewhavenLCDSerial::moveRight(){
	commandShort(LCD_MOVERIGHT);
}

// Backspace
void NewhavenLCDSerial::backspace(){
	commandShort(LCD_BACKSPACE);
}

// Set Baud Rate
void NewhavenLCDSerial::setBaudRate(int baud){
	int arg;
  switch (baud) {
  case 300 : 
    arg = 1;
    break;
  case 1200 : 
    arg = 2;
    break;
  case 2400 : 
    arg = 3;
    break;
  case 9600 : 
    arg = 4;
    break;
  case 14400 : 
    arg = 5;
    break;
  case 19200 : 
    arg = 6;
    break;
  case 57600 : 
    arg = 7;
    break;
  case 115200 : 
    arg = 8;
    break;
  default:
    arg = 0;
  }
  if (arg > 0){
   write(LCD_CMD_PREFIX);
   write(LCD_SETBAUDRATE);
   write(arg);
   delay(11);
  }
}


// Display Baud Rate
void NewhavenLCDSerial::displayBaudRate(){
	commandVeryLong(LCD_DISPLAYBAUDRATE);
}

// Display I2C Address
void NewhavenLCDSerial::displayI2CAddress(){
	commandVeryLong(LCD_DISPLAYI2CADDR);
}



// Set cursor to specific row and col. 0,0 is top left.
void NewhavenLCDSerial::setCursor(int row, int col){
	int row_offsets[2] = {0x00, 0x40};
	if (((row >= 0) && (row <= 1)) && ((col >= 0) && (col <= 15))){
	  write(LCD_CMD_PREFIX);
	  write(LCD_SETCURSOR);
	  write(row_offsets[row] | col);

	}
}

// Creates custom characters 8 char limit
// Input values start with 1
/*void NewhavenLCDSerial::createChar(int location, uint8_t charmap[]){
	location -= 1;
	location &= 0x07;
  for (int i=0; i<8; i++){
    command(LCD_SETCGRAMADDR | (location << 3) | i);
    write(charmap[i]);
  }
}
*/

// Prints custom character
// Input values start with 1
/*void NewhavenLCDSerial::printCustomChar(int num){
	write((num - 1));
}
*/

// PRIVATE FUNCTIONS

// Functions for sending the special command values
void NewhavenLCDSerial::commandShort(uint8_t value){
	write(LCD_CMD_PREFIX);
	write(value);
	delayMicroseconds(LCD_DELAY_SHORT);
}

void NewhavenLCDSerial::commandLong(uint8_t value){
	write(LCD_CMD_PREFIX);
	write(value);
	delayMicroseconds(LCD_DELAY_LONG);
}

void NewhavenLCDSerial::commandVeryLong(uint8_t value){
	write(LCD_CMD_PREFIX);
	write(value);
	delay(LCD_DELAY_VERYLONG);
}

