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
const LiquidCrystal LCD(3, 4, 6, 7, 8, 9);

/**
 * The time in milliseconds that should be waited after "reprinting" onto the LCD used.
 * Writing to the LCD can cause flickering otherwise, if written onto the LCD too often.
 */
const int FLICKER_PREVENTION_TIME = 100;

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup() {
  // initializes the interface to the LCD with 4 rows and 20 chars per row
  LCD.begin(20, 4);

}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex01-5#lcdLogic()
 */
void loop() {
  // The current value of the analog pin A0.
  int analogValue = analogRead(A0);

  lcdLogic(analogValue);
  
}

/*
 * The logic used to control the LCD.
 * 
 * This method completely blocks other logic-implementation for the time
 * specified in {@link ex01-6#FLICKER_PREVENTION_TIME}.
 * 
 * @param mAnalogValue
 *                An integer value which gets printed onto the LCDs screen.
 */
void lcdLogic(const int mAnalogValue){
  LCD.clear();
  LCD.print("Analog 0: " + String(mAnalogValue));
  delay(FLICKER_PREVENTION_TIME);
  
}

