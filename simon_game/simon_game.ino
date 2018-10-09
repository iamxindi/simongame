#include <NewhavenLCDSerial.h>
#include <SoftwareSerial.h>
#include "notes.h"


const int ledPins[] = {2, 3, 4, 5}; // set up led pins
const int buttonPins[] = {6, 7, 8, 9}; // set up button pins
const int max_level = 100; // max possible level
const int max_life = 5; // start with 5 lives
int velocity = 300; // velocity - time in between 
int level = 3; // number of lights in a sequence
int life = 5; // init life 
int game_sequence[max_level]; // the pattern of simon game
int user_sequence[max_level]; // the pattern that user plays
int count = 0; // init button press count for later
int wrong = 0; // init wrong flag for later

//set up tune
int melody[] = {NOTE_E4, NOTE_FS4, NOTE_B4, NOTE_CS5, NOTE_D5, NOTE_FS4, NOTE_E4, NOTE_CS5, NOTE_B4, NOTE_FS4, NOTE_D5, NOTE_CS5};
int noteDurations = 1000 / 8;

//set up lcd display
int rows = 2;      // number of rows
int cols = 16;     // number of columns
NewhavenLCDSerial lcd(10);


int timer;  //debounce timer

enum btn {ON, DEBOUNCE, OFF}; // our own type for button states

btn btnstate[4];    // variable to store state

void setup() {
  // initialize led pins as output
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  // initialize button pins as input
  pinMode(buttonPins[0], INPUT_PULLUP); 
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(buttonPins[2], INPUT_PULLUP);
  pinMode(buttonPins[3], INPUT_PULLUP);
  
  //to start, all the lights are on
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[2], HIGH);
  digitalWrite(ledPins[3], HIGH);

  lcd.clear();              // clear LCD

  Serial.begin(9600);
  
  //print welcome message
  lcdPrintWelcome();


}

void lcdPrintWelcome() {

  lcd.setCursor(0, 5);     // row 0, column 5
  lcd.print("WELCOME");    // display text
  delay(500);            // wait
  lcd.clear();             // clear LCD

  lcd.setCursor(0, 0);
  lcd.print("Press any button to begin");
  lcd.setCursor(1, 0);
  lcd.print("to begin");
  lcd.display();
  delay(5000);
  lcd.clear();             // clear LCD

}

void lcdPrintStart() {
  
  // the number of level 
  int level_num = level - 2;
  
  //print level number on lcd
  lcd.setCursor(0, 0);     // row 0, column 0
  lcd.print("Level");    // display text
  lcd.setCursor(1, 0);     // row 1, column 0
  lcd.print(level_num);    // display text
  delay(2000);            // wait
  lcd.clear();             // clear LCD
  lcd.setCursor(0, 0);     // row 0, column 0
  lcd.print("Your Life");    // display text
  // print symbols for number of life left
  for (int i = 0; i < life; i++) {
    lcd.setCursor(1, i);
    lcd.print("*");
  };
  delay(2000);            // wait
  lcd.clear();             // clear LCD


}

// show this before showing game sequence
void lcdPrintShow() {

  lcd.setCursor(0, 0);     // row 0, column 0
  lcd.print("Showing Sequence...");    // display text
  delay(3000);            // wait
  lcd.clear();             // clear LCD

}

// show this when user press wrong button
void WrongOne() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Life -1");    // display text
  life -= 1;
  for (int i = 0; i < life; i++) {
    lcd.setCursor(1, i);
    lcd.print("*");
  };
  delay(2000);            // wait
  lcd.clear();             // clear LCD


}

// set up game states

enum game {INIT, GENERATE, PLAY, LISTEN, WRONG};
game gamestates;

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 4; i++) { // for the four buttons

    switch (gamestates) {

      case INIT:
        // four lights are on
        digitalWrite(ledPins[0], HIGH);
        digitalWrite(ledPins[1], HIGH);
        digitalWrite(ledPins[2], HIGH);
        digitalWrite(ledPins[3], HIGH);
 
        Serial.println("In init"); // for debug
        level = 3; //when a new game starts, initialize num of steps to 3
        life = max_life; //when a new game starts, initialize num of steps to max_life


        if (digitalRead(buttonPins[i]) == LOW) { //if any button is pressed
          Serial.println("button pressed"); // debug
          int foo = millis(); // returns the current time in ms since the program started
          randomSeed(foo); // make it really random
          gamestates = GENERATE; // go prepare for the game
        } else {
          break;
        }

      case GENERATE:
        allOff(); // all lights off
        Serial.println("in generate"); // debug

        for (int i = 0; i < level; i++) { // create a "level" number sequence of lights
          game_sequence[i] = ledPins[random(0, 4)];
          Serial.println(game_sequence[i]);
        };
        gamestates = PLAY; // start play
        break;



      case PLAY:
        Serial.println("In play mode");//debug
        delay(500);
        lcdPrintStart(); // print message
        show_sequence(); // show pattern
        gamestates = LISTEN; // wait for input
        break;


      case LISTEN:
        wrong = 0; // set up flag for wrong input
        ReadButton(); // read button input with debounce
        Serial.println(wrong); //debug
        if (wrong == 1) { // if user is wrong
          count = 0; // re init count of button press to 0
          gamestates = WRONG; // go to wrong state

        } else if (count >= level) { // if user pressed enough buttons
          Serial.println("count > level"); //debug
          //Serial.println("1");
          noTone(12); // silence
          count = 0; // re init count of button press to 0
          level += 1; // will go to next level
          delay(200);
          gamestates = GENERATE; // go to generate sequence for the next level
        }
        break;



      case WRONG:
        noTone(12); //silence
        Serial.println("wrong entered"); // for debug
        delay(50);
        WrongOne(); // display wrong msg
        if (life > 0) { // if there's still life
          gamestates = GENERATE; // continue game in same level
        } else { // if no life is left
          allOff(); //turn all lights off
          lcd.setCursor(0, 0);     // row 0, column 0
          lcd.print("Game Over! Press any");
          lcd.setCursor(1, 0);     // row 1, column 0
          lcd.print("button to start again");
          delay(2000);
          lcd.clear();
          gamestates = INIT; // wait for button input to start a new game

        }

    }
  }
}

void ReadButton() {

  for (int i = 0; i < 4; i++) { // for the four buttons
    int reading = digitalRead(buttonPins[i]);  // check pin reading

    switch (btnstate[i]) {

      case OFF:
        if (reading == LOW) { // button press?
          btnstate[i] = DEBOUNCE;
          timer = 5;
        } break;
      case DEBOUNCE:
        if (reading == LOW) {  // if button reading remains low
          if (timer <= 0) {   // and the time elapses
            btnstate[i] = ON;       // this is a "real" button press"
            digitalWrite(ledPins[i], HIGH);
            user_sequence[count] = ledPins[i];
            Serial.println(count);
            if (user_sequence[count] == game_sequence[count]) {
              Serial.println("Right one");
              tone(12, melody[0]); //play the sound for right one
              lcd.setCursor(0, 0);     // row 0, column 0
              lcd.print(count + 1); // display the current number of count
              lcd.setCursor(1, 0);
              lcd.print("correct"); // display "correct"
              delay(100);
              lcd.clear();


            } else {
              Serial.println("Wrong one");
              wrong = 1; // wrong flag set to 1
              tone(12, melody[5]); //play the sound for wrong one
              lcd.setCursor(0, 0);     // row 0, column 0
              lcd.print("WRONG");  //display "wrong"
              delay(100);
              lcd.clear();


            };
            count += 1; // increase button press count
            break;

          } else {
            timer--; // if time hasn't elapsed, but button is still low, keep waiting
          }

        }

        else btnstate[i] = OFF;
        break;

      case ON:
        if (reading == HIGH) { //button is not pressed
          btnstate[i] = OFF;
          digitalWrite(ledPins[i], LOW); // light is off
          noTone(12); // no sound
        }

    }
  }
}
void allOff() { // turn all lights off
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW);
  digitalWrite(ledPins[3], LOW);

}

void show_sequence()
{


  Serial.println("Showing sequence..."); //debug
  lcdPrintShow();// show message on lcd
  delay(1000);

  for (int i = 0; i < level; i++) //i = level
  // for lights in the current pattern, turn lights on and off
  {
    digitalWrite(game_sequence[i], HIGH); 
    delay(velocity);
    digitalWrite(game_sequence[i], LOW);
    delay(velocity);
  };
  // finish pattern, all lights off
  allOff();
  Serial.println("Your turn!"); //debug
  // display message on lcd
  lcd.setCursor(0, 0);
  lcd.print("Your Turn!");
  delay(300);
  lcd.clear();

}
