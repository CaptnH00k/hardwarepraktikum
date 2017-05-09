/*
 * In this exercise the ultrasonic sensor should be used. Aswell
 * as the implementation of a function which should return the distance
 * to an object in centimeters.
 * 
 * Connections:
 * 
 * <ULTRASONIC>
 * US1: PIN 12
 * 
 * <LCD>:
 * R/S: PIN 2
 * R/W: None
 * E  : PIN 3
 * DB4: PIN 4
 * DB5: PIN 5
 * DB6: PIN 7
 * DB7: PIN 8
 * 
 * 
 * @author Ativelox {@literal ativelox.dev@web.de}
 * 
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
const LiquidCrystal lcd(2, 3, 4, 5, 7, 8);

/**
 * The time in milliseconds that should be waited after "reprinting" onto the LCD used.
 * Writing to the LCD can cause flickering otherwise, if written onto the LCD too often.
 */
const int FLICKER_PREVENTION_TIME = 100;

/**
 * Holds the value of the pin to which the ultrasonic sensor is connected to.
 */
const int8_t ULTRASONIC_SENSOR = 12;

/**
 * Holds the time in microseconds of how long to wait per impulse.
 */
const int8_t IMPULSE_WAIT_US = 15;

/**
 * Holds the value of how long to wait until checking the value of the ultrasonic sensor again.
 */
const uint8_t ULTRASONIC_MEASURE_WAIT = 50;

/**
 * Holds the value of when to time the measure out, e.g. if the ultrasonic sensor didn't 
 * write a pulse for {@ex02-6#ULTRASONIC_MEASURE_TIMEOUT} microseconds, there was no
 * obstacle detected.
 */
const uint32_t ULTRASONIC_MEASURE_TIMEOUT = 30000;

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup(){
  
  //set the ultrasonic sensor up, so it can measure distances.
  pinMode(ULTRASONIC_SENSOR, OUTPUT);

  impulse(ULTRASONIC_SENSOR);
  
  pinMode(ULTRASONIC_SENSOR, INPUT);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-6#debugLCDLogic()
 */
void loop(){
  debugLCDLogic();

  // prevents flickering of the lcd.
  delay(FLICKER_PREVENTION_TIME);
  
}

/**
 * Prints the distance to the obstacle in front of the robot onto the LCD.
 * Used for debugging purposes.
 */
int debugLCDLogic(){

  // Clear display each time this method gets called, so the 
  // old LCD-display doesn't get overwritten
  lcd.clear();

  // Sets the cursor to the first row at the first char
  lcd.setCursor(0, 0);

  // Print the distance to the obstacle in front of the robot onto the lcd.
  lcd.print("Distance: " + String(measureDistance(ULTRASONIC_SENSOR)));
  
}

/**
 * Measures the distance to objects in front of the given sensor. Returns
 * them in centimeter.
 * 
 * @param mUltrasonicSensor
 *              The sensor from which to get the data for obstacle distances.
 */
int measureDistance(const int8_t mUltrasonicSensor){

  // holds the value of how long we waited for the pulse to 
  // start (basically a timeout)
  uint32_t wait_us = 0;

  while(true){
    
    //pulse started
    if(digitalRead(mUltrasonicSensor) == HIGH){
      
      // start measuring.
      uint32_t time_before = micros();

      // wait until the pulse ended.
      while(digitalRead(mUltrasonicSensor) == HIGH){
        delayMicroseconds(ULTRASONIC_MEASURE_WAIT);
  
      }

      // calculate the pulse time
      uint32_t impulseTime = micros() - time_before;
  
      return (impulseTime / 58);

    // no HIGH signal from the ultrasonic sensor, wait up to 30ms, 
    // if there's wasn't a HIGH signal in the given time return -1
    }else{
      delayMicroseconds(ULTRASONIC_MEASURE_WAIT);
      wait_us = wait_us + ULTRASONIC_MEASURE_WAIT;

      if(wait_us > ULTRASONIC_MEASURE_TIMEOUT){
        return -1;
        
      }
    }
  }
}

/**
 * Sends an impulse to the given pin, with a specified wait 
 * in {@link ex02-6#IMPULSE_WAIT_US} from high to low.
 * 
 * @param mUltrasonicSensorPin
 *              The pin which receives the impulse.
 */
void impulse(const int8_t mUltrasonicSensorPin){
  
  //reset the signal on the pin to low
  digitalWrite(mUltrasonicSensorPin, LOW);

  //now set it to high
  digitalWrite(mUltrasonicSensorPin, HIGH);

  //now wait for the specified pulse time
  delayMicroseconds(IMPULSE_WAIT_US);

  //finally write 0 to the pin so the pulse is complete
  digitalWrite(mUltrasonicSensorPin, LOW);
 }

