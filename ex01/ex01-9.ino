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
 * A constant holding the reference to a LiquidCrystal object (the lcd on the board). 
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

/**
 * The number of the pin which is associated with the red LED.
 */
const int RED_LED = 10;

/**
 * The number of the pin which is associated with the yellow LED.
 */
const int YELLOW_LED = 11;

/**
 * The number of the pin which is associated with the green LED.
 */
const int GREEN_LED = 12;

const int AUDIO_PIN = 13;

/*
 * The time in milliseconds for how long the red LED is turned on. 
 */
const int RED_LED_ON_TIME = 6000;

/*
 * The time in milliseconds for how long the yellow LED is turned on.
 */
const int YELLOW_LED_ON_TIME = 1000;

/*
 * The time in milliseconds for how long the green LED is turned on.
 */
const int GREEN_LED_ON_TIME = 3000;

/*
 * The time in milliseconds when the Logic of the LEDs pattern gets reset.
 */
const int LED_LOGIC_RESET = 10000;

/**
 * 
 */
const int RED_FREQ = 100;

/**
 * 
 */
const int GREEN_FREQ = 400;

/**
 * A boolean indicating whether the red LED is on or not.
 */
bool redIsOn = false;

/**
 * A boolean indicating whether the yellow LED is on or not.
 */
bool yellowIsOn = false;

/**
 * A boolean indicating whether the green LED is on or not.
 */
bool greenIsOn = false;

bool redSoundOn = false;

bool greenSoundOn = false;

/**
 * A boolean determining if the pattern has started or not.
 */
bool patternStart = false;

/**
 * A long storing the value of the milliseconds at the method 
 * start of {@link ex01-8#ledLogic())
 */
unsigned long counter = 0;

/**
 * Counts the milliseconds passed since the start of this application. 
 * Will reset after a given amount of time (estimated reset after 50 days)
 */
int tickCounter;

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup() {

  // set to {@link ex01-7#FLICKER_PREVENTION_TIME}, so the content can 
  // be written on the LCD instantly for the first time.
  tickCounter = FLICKER_PREVENTION_TIME;
  
  // initializes the interface to the LCD with 4 rows and 20 chars per row
  lcd.begin(20, 4);

  // Set the LCDs, so they will behave as outputs.
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex01-7#lcdLogic()
 * @see ex01-7#ledLogic()
 */
void loop() {
  
  // The current milliseconds passed since this applications start
  unsigned long time_before = millis();

  // The value of the analog pin A0
  int analogValue = analogRead(A0);

  ledLogic(analogValue);

  lcdLogic(analogValue);
  
}

/**
 * Turns the given LED on.
 * 
 * @param mLED
 *          The pin number associated with LED.
 *          
 * @param mLEDStatus 
 *          An array containing booleans which are the current status of each flag
 *            
 * @see ex01-7#ledStatus            
 */
void turnOn(const int mLED){
  // if the LED is off turn it on, and return true, so 
  // the caller knows the "turning on" succeeded.
     if(mLED == RED_LED){
        if(redIsOn == false){
           digitalWrite(mLED, HIGH);
           redIsOn = true;
        }
     }
     if(mLED == YELLOW_LED){
           if(yellowIsOn == false){
           digitalWrite(mLED, HIGH);
           yellowIsOn = true;
        }
     }
     if(mLED == GREEN_LED){
           if(greenIsOn == false){
           digitalWrite(mLED, HIGH);
           greenIsOn = true;
        }
        
     }
}


/**
 * Turns the given LED off.
 * 
 * @param mLED
 *          The pin number associated with LED.
 *          
 * @param mLEDStatus 
 *          An array containing booleans which are the current status of each flag
 *            
 * @see ex01-7#ledStatus            
 */
void turnOff(const int mLED){

  // if the LED is on turn it off, and return false, so
  // the caller can update the LED status.
     if(mLED == RED_LED){
        if(redIsOn == true){
           digitalWrite(mLED, LOW);
           redIsOn = false;
        }
     }
     if(mLED == YELLOW_LED){
           if(yellowIsOn == true){
           digitalWrite(mLED, LOW);
           yellowIsOn = false;
        }
     }
     if(mLED == GREEN_LED){
           if(greenIsOn == true){
           digitalWrite(mLED, LOW);
           greenIsOn = false;
        }  
    }
}

void soundOn(const int mLED){
  if(mLED == RED_LED){
      tone(AUDIO_PIN, RED_FREQ);
      
  }
  if(mLED == GREEN_LED){
      tone(AUDIO_PIN, GREEN_FREQ);
  }
  
}

void soundOff(const int mLED){
  noTone(AUDIO_PIN);
  
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

/*
 * The logic needed to control the LCDs features (printing, clearing, etc...)
 * 
 * @param mTimeSpan
 *            The time passed by for the callers method call up until calling this method.
 *            
 * @param mAnalogValue
 *            The value to be printed onto the LCD as the current analog pins value.
 */
void lcdLogic(const int mAnalogValue){

  // The current value of the analog pin A0.
  int analogValue = analogRead(A0);

  String buttonRepresentation = "";
  if(redIsOn){
    buttonRepresentation = buttonRepresentation + "red";
  }
  if(yellowIsOn){
    if(buttonRepresentation == "red"){
      buttonRepresentation = buttonRepresentation + "+";
    }
    buttonRepresentation = buttonRepresentation + "yellow";
  }
  if(greenIsOn){
    buttonRepresentation = buttonRepresentation + "green";
   
  }

  // Clear display each time this method gets called, so the 
  // old LCD-display doesn't get overwritten
  lcd.clear();

  // Sets the cursor to the first row at the first char
  lcd.setCursor(0,0);
  
  // Prints the value given onto the LCD  
  lcd.print("Analog 0: " + String(mAnalogValue));

  //Sets the cursor to the second row at the first char
  lcd.setCursor(0,1); // set cursor to second line

  // Prints the value of the button which got pressed at the method call  
  lcd.print("button: " + buttonToString(getCurrentButton(mAnalogValue)));

  // Sets the cursor to the third row at the first char
  lcd.setCursor(0,2);

  // Prints a string representation of all the LEDs which are currently on
  lcd.print("led: " + buttonRepresentation);

  delay(FLICKER_PREVENTION_TIME);
}

/*
 * The logic needed to control the LEDs flashing pattern.
 * Also updates the LED status of the LEDs when they are
 * turned on, off.
 * 
 * @param mTime
 *          The time in milliseconds passed before the logic reset for this method.
 */
void ledLogic(const int mButtonValue){

  String button = buttonToString(getCurrentButton(mButtonValue));

  //only make the red led light, if the first button didn't get pressed
  if(button != "S1" && !patternStart){
    turnOn(RED_LED);
    soundOn(RED_LED);
    turnOff(YELLOW_LED);
    counter = millis();
    return;
    
  }else{
    patternStart = true;
    unsigned long time_now = (millis() - counter);

    if(time_now < YELLOW_LED_ON_TIME){
      turnOn(YELLOW_LED);
      
    }else if(time_now < (YELLOW_LED_ON_TIME + GREEN_LED_ON_TIME)){
      turnOff(RED_LED);
      soundOff(RED_LED);
      
      turnOff(YELLOW_LED);

      turnOn(GREEN_LED);
      soundOn(GREEN_LED);
      
    }else if(time_now < (2 * YELLOW_LED_ON_TIME + GREEN_LED_ON_TIME)){
     turnOff(GREEN_LED);
     soundOff(GREEN_LED);
     
     turnOn(YELLOW_LED);
     
    }else{
      counter = millis();
      patternStart = false;
    }
    
  }
}

