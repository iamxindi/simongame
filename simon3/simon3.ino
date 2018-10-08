#include <NewhavenLCDSerial.h>
#include <SoftwareSerial.h>


const int ledPins[] = {2, 3, 4, 5};
const int buttonPins[] = {6, 7, 8, 9};
const int max_level = 100;
int velocity = 300;
int level = 3;
int life = 5;
int game_sequence[max_level];
int user_sequence[max_level];
int count = 0;
int wrong = 0;

//lcd display
int rows = 2;      // number of rows
int cols = 16;     // number of columns
char line0[20];
char line1[20];


NewhavenLCDSerial lcd(12); 


int timer;  //debounce

enum btn{ON,DEBOUNCE,OFF}; // our own type for button states

btn btnstate[4];    // variable to store state

void setup() {
   pinMode(ledPins[0], OUTPUT);// initialize the LED pin as an output
   pinMode(ledPins[1], OUTPUT);
   pinMode(ledPins[2], OUTPUT);
   pinMode(ledPins[3], OUTPUT);  
   pinMode(buttonPins[0], INPUT_PULLUP);
   pinMode(buttonPins[1], INPUT_PULLUP);
   pinMode(buttonPins[2], INPUT_PULLUP);
   pinMode(buttonPins[3], INPUT_PULLUP);

   digitalWrite(ledPins[0], HIGH);
   digitalWrite(ledPins[1], HIGH);
   digitalWrite(ledPins[2], HIGH);
   digitalWrite(ledPins[3], HIGH);

   lcd.clear();              // clear LCD

   Serial.begin(9600); 
   lcdPrintWelcome();


}

void lcdPrintWelcome(){ 
  
  lcd.setCursor(0,5);      // row 0, column 5
  lcd.print("WELCOME");    // display text
  delay(500);            // wait
  lcd.clear();             // clear LCD
  
  lcd.setCursor(0,0);
  lcd.print("Press any button to begin");
  lcd.setCursor(1,0);
  lcd.print("to begin"); 
  lcd.display();
  delay(5000);
  lcd.clear();             // clear LCD
 
      
}

void lcdPrintStart(){

  int level_num = level - 2;
  
  lcd.setCursor(0,0);      // row 0, column 5
  lcd.print("Level");    // display text
  lcd.setCursor(1,0);      // row 0, column 5
  lcd.print(level_num);    // display text
  delay(2000);            // wait
  lcd.clear();             // clear LCD
  lcd.setCursor(0,0);      // row 0, column 5
  lcd.print("Your Life");    // display text
  for (int i=0; i<life; i++){
    lcd.setCursor(1,i);
    lcd.print("*");  
    };
  delay(2000);            // wait
  lcd.clear();             // clear LCD

    
}

void lcdPrintShow(){ 
  
  lcd.setCursor(0,0);      // row 0, column 5
  lcd.print("Showing Sequence...");    // display text
  delay(3000);            // wait
  lcd.clear();             // clear LCD
    
}


void WrongOne(){ 
  
  lcd.clear(); 
  lcd.setCursor(0,0);      // row 0, column 5
  lcd.print("Life -1");    // display text
  for (int i=0; i<life; i++){
    lcd.setCursor(1,i);
    lcd.print("*");  
   };
  delay(50);
  life -= 1;
  for (int i=0; i<life; i++){
    lcd.setCursor(1,i);
    lcd.print("*");  
   };
  delay(2000);            // wait
  lcd.clear();             // clear LCD

    
}

enum game{INIT, GENERATE, PLAY, LISTEN, WRONG};
game gamestates;

void loop() {
  updateDisplay(); 
  // put your main code here, to run repeatedly:

  for(int i=0; i<4; i++){
  
   switch(gamestates){

    case INIT:

        Serial.println("In init");      
         

      if(digitalRead(buttonPins[i])==LOW){ 
        Serial.println("button pressed");
        int foo = millis(); // returns the current time in ms since the program started  
        randomSeed(foo);
        gamestates = GENERATE;
        }else{
        break;
        }

     case GENERATE:
        allOff();   
        Serial.println("in generate");

        for (int i=0; i<level; i++){
        game_sequence[i] = ledPins[random(0,4)];
        Serial.println(game_sequence[i]); 
        };
        gamestates = PLAY;
        break;
 
    
   
    case PLAY:
    Serial.println("In play mode");
    delay(500);  
    lcdPrintStart();
    show_sequence();
    gamestates = LISTEN;  
    break;
   
    
    case LISTEN:
    wrong = 0;   
    ReadButton();
    Serial.println(wrong);
    if(wrong == 1){
      count = 0;
      gamestates = WRONG;
      
      }else if(count >= level){
     Serial.println("count > level");
           //Serial.println("1");
          count = 0;
           level+=1;
           delay(200);
           gamestates = GENERATE;
       }
      break;
      


      case WRONG:
      Serial.println("wrong entered");
      delay(50);
      WrongOne();
      gamestates = GENERATE;
    
       
    }
  }
}

void ReadButton() {

  for(int i = 0; i < 4; i++){ // for the four buttons
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
          if(user_sequence[count] == game_sequence[count]){
            Serial.println("Right one");
            lcd.setCursor(0,0);      // row 0, column 5
            lcd.print(count + 1);
            lcd.setCursor(1,0);
            lcd.print("correct");
            delay(100);
            lcd.clear();
            
            
            }else{
              Serial.println("Wrong one");
              wrong = 1;
              lcd.setCursor(0,0);      // row 0, column 5
              lcd.print("WRONG");
              delay(100);
              lcd.clear();
              
              
              }; 
              count += 1;
              break;                    
        
        } else {timer--;}     // if time hasn't elapsed, but button is still low, keep waiting
     
     }  
     
     else btnstate[i] = OFF;     
     break;
         
    case ON:
      if (reading == HIGH) {
        btnstate[i] = OFF;
        digitalWrite(ledPins[i],LOW);
      }
    
   }
  }
}
void allOff(){
   digitalWrite(ledPins[0], LOW);
   digitalWrite(ledPins[1], LOW);
   digitalWrite(ledPins[2], LOW);
   digitalWrite(ledPins[3], LOW);
  
  }

void show_sequence()
  {
 
    
    Serial.println("Showing sequence...");
    lcdPrintShow();
    delay(1000);

  for (int i = 0; i < level; i++) //i = level
  {
    digitalWrite(game_sequence[i], HIGH);
    delay(velocity);
    digitalWrite(game_sequence[i], LOW);
    delay(velocity);
  };
  allOff();
  Serial.println("Your turn!");
  lcd.setCursor(0,0);
  lcd.print("Your Turn!");
  delay(50);
  lcd.clear();

}


void updateDisplay() {
   lcd.setCursor(0,0);
   lcd.print(line0);
   lcd.setCursor(1,0);
   lcd.print(line1);
}
