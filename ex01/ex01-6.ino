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

// include the library used to control the LCD attached to the arduino board. 
#include <LiquidCrystal.h>

/*
 * A constang holding the reference to a LiquidCrystal object (the lcd on the board). 
 * The constructer used is specified as LiquidCrystal(rs, enable, d4, d5, d6, d7).
 * 
 * See the available documentation here:
 * https://www.arduino.cc/en/Reference/LiquidCrystalConstructor
 */
const LiquidCrystal lcd(3, 4, 6, 7, 8, 9);

/**
 * The time in milliseconds that should be waited after "reprinting" onto the LCD used.
 * Writing to the LCD can cause flickering otherwise, if written onto the LCD too often.
 */
const int FLICKER_PREVENTION_TIME = 100;

/**
 * The value needed to identify the first button by the value of the analog pin.
 */
const int FIRST_BUTTON_THRESHOLD = 121;

/**
 * The value needed to identify the second button by the value of the analog pin.
 */
const int SECOND_BUTTON_THRESHOLD = 357;

/**
 * The value needed to identify the third button by the value of the analog pin.
 */
const int THIRD_BUTTON_THRESHOLD = 575;

/**
 * The value needed to identify the fourth button by the value of the analog pin.
 */
const int FOURTH_BUTTON_THRESHOLD = 753;

/**
 * The value needed to identify the fifth button by the value of the analog pin.
 */
const int FIFTH_BUTTON_THRESHOLD = 927;

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup() {
  // initializes the interface to the LCD with 4 rows and 20 chars per row
  lcd.begin(20, 4);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex01-6#lcdLogic()
 */
void loop() {
  // the value of the analog pin A0.
  int analogValue = analogRead(A0);

  lcdLogic(analogValue);
}

/*
 * The logic used to control the LCD. See method body for more information.
 * 
 * This method completely blocks other logic-implementation for the time
 * specified in {@link ex01-6#FLICKER_PREVENTION_TIME}.
 * 
 * @param mAnalogValue
 *              An integer value which gets printed onto the LCDs screen.
 */
void lcdLogic(const int mAnalogValue) {
  
  // Clear display each time this method gets called, so the 
  // old LCD-display doesn't get overwritten
  lcd.clear();

  // Sets the cursor to the first row at the first char
  lcd.setCursor(0,0);

  // Prints the value given onto the LCD
  lcd.print("Analog 0: " + String(mAnalogValue));

  //Sets the cursor the second row at the first char
  lcd.setCursor(0,1);

  // Prints the value of the button which got pressed at the method call
  lcd.print("button: " + buttonToString(getCurrentButton(mAnalogValue)));
  delay(FLICKER_PREVENTION_TIME);
}

/**
 * Converts the given integer value associated with a button to
 * a String in a fashionable way.
 * 
 * @param mButtonValue
 *              An integer value associated with a specific button.
 */
String buttonToString(const int mButtonValue) {

  // no button was pressed
  if(mButtonValue == 0){
    return "-";

  } else {
    return "S"+String(mButtonValue);
    
  }
}

/*
 * Gets the current integer value associated with the button wanted.
 * Does so by extracting the button out of the analog value given.
 * Those values aren't constant, thus thresholds are used to
 * identify the appropriate button.
 * 
 * Because values are not static, we always use the middle between
 * the value for this button and the next higher.
 * Eg: Button1: 0, Button2: 241 ((241 + 0) / 2) + 0 = 121 
 * 
 * @param mAnalogValue
 *              The value of the analog pin, used to identify the 
 *              button pressed
 * 
 */
int getCurrentButton(const int mAnalogValue) {
  
  if(mAnalogValue < FIRST_BUTTON_THRESHOLD) {
    return 1;
    
  }else if(mAnalogValue < SECOND_BUTTON_THRESHOLD) {
    return 2;
    
  }else if(mAnalogValue < THIRD_BUTTON_THRESHOLD) {
    return 3;
    
  }else if(mAnalogValue < FOURTH_BUTTON_THRESHOLD) {
    return 4;
    
  }else if(mAnalogValue < FIFTH_BUTTON_THRESHOLD) {
    return 5;

  // no button pressed
  }else {
    return 0;
    
  }
}
