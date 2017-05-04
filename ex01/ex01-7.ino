// display test: write some data to the lcd

/* Connections:
 R/S : Pin 3
 R/W : not connected
 E   : Pin 4
 DB4 : Pin 6
 DB5 : Pin 7
 DB6 : Pin 8
 DB7 : Pin 9
*/

// include LCD functions:
#include <LiquidCrystal.h>

// define the LCD screen
LiquidCrystal lcd(3, 4, 6, 7, 8, 9);

// global variable, stores the result from analog pin
int analogValue;
boolean onRed = false;
boolean onYellow = false;
boolean onGreen = false;
int millimod;
void setup()
{
  // LCD has 4 lines with 20 chars
  lcd.begin(20, 4);

  pinMode(10, OUTPUT); // red
  pinMode(11, OUTPUT); // yellow
  pinMode(12, OUTPUT); // green

}


void loop()
{
  // Because values are not static, we always use the middle between
  analogValue = analogRead(A0);
  lcd.clear(); // Clear display to not overwrite stupid things
  lcd.setCursor(0,0); // Set cursor to first line
  lcd.print("Analog 0: " + String(analogValue)); // print out the analog value
  lcd.setCursor(0,1); // set cursor to second line
  lcd.print("button: " + whichButtonIsPressedAsString()); // print out which button is pressed
  // wait for 100 ms (reduces display flickering)
  lcd.setCursor(0,2); // set cursor to second line
  lcd.print("led: " + whichLEDSONAsText());
  millimod = millis() % 10000;

  if(millimod < 5000) {
    turnYellowOFF();
    turnRedON(); 
  }else if(millimod < 6000){
    turnYellowON();
  }else if(millimod < 9000){
    turnRedOFF();
    turnYellowOFF();
    turnGreenON();
  }else {
    turnGreenOFF();
    turnYellowON();
  }
  delay(100);
}

String whichLEDSONAsText() {
  if(onRed == true && onYellow == false) {
    return "red        ";
  }else if(onRed == true && onYellow == true) {
    return "red+yellow ";
  }else if(onYellow == true) {
    return "yellow     ";
  }else if(onGreen == true) {
    return "green      ";
  }
}

void turnRedON() {
  if(onRed == false) {
    digitalWrite(10, HIGH);
    onRed = true;
  }
}

void turnRedOFF() {
  if(onRed == true) {
    digitalWrite(10, LOW);
    onRed = false;    
  }
}


void turnYellowON() {
  if(onYellow == false) {
    digitalWrite(11, HIGH);
    onYellow = true;
  }
}

void turnYellowOFF() {
  if(onYellow == true) {
    digitalWrite(11, LOW);
    onYellow = false;
  }
}

void turnGreenON() {
  if(onGreen == false) {
    digitalWrite(12, HIGH);
    onGreen = true;
  }
}

void turnGreenOFF() {
  if(onGreen == true) {
    digitalWrite(12, LOW);
    onGreen = false;    
  }
}
// Returns which button is pressed as a string. "S1-5" => Button 1-5, "-"=> no button is pressed
String whichButtonIsPressedAsString() {
  int buttonPressed = whichButtonIsPressed();
  if(buttonPressed == 0){
    return "-";
  } else {
    return "S"+String(buttonPressed);
  }
}

// Returns which button is pressed. 1-5 => Button 1-5, 0=> no button is pressed
int whichButtonIsPressed()
{
  // Because values are not static, we always use the middle between
  // the value for this button and the next higher.
  // Eg: Button1: 0, Button2: 241 ((241 + 0) / 2) + 0 = 121 
  if(analogValue < 121) {
    return 1;
  }else if(analogValue < 357) {
    return 2;
  }else if(analogValue < 575) {
    return 3;
  }else if(analogValue < 753) {
    return 4;
  }else if(analogValue < 927) {
    return 5;
  }else {
    // no button pressed
    return 0;
  }
}


/* Usefull LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);

write onto lcd, starting at current position:
  lcd.print("abc");

clear the lcd (this writes " " into all positions and is therefore slow):
If only specific areas should be cleared use a mix of setCursor and print(" ") instead
  lcd.clear();
*/
