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
int test = 5;

void setup()
{
  // LCD has 4 lines with 20 chars
  lcd.begin(20, 4);
}


void loop()
{
  lcd.clear();
  lcd.print("button: " + whichButtonIsPressedAsString());
   // wait for 100 ms (reduces display flickering)
  delay(100);
}

String whichButtonIsPressedAsString() {
  int buttonPressed = whichButtonIsPressed();
  if(buttonPressed == 0){
    return "-";
  } else {
    return "S"+String(buttonPressed);
  }
}

int whichButtonIsPressed()
{
  analogValue = analogRead(A0);
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
