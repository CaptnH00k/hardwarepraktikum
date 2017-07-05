// TurnRate Sensor: read data from analog gyroscope, output heading on lcd

/* Connections:
 R/S : Pin 11
 R/W : not connected
 E   : Pin 12
 DB4 : Pin 13
 DB5 : Pin A0
 DB6 : Pin A1
 DB7 : Pin A2
 
 
 Turn-Rate Sensor: A3
*/
// include LCD functions:
#include <LiquidCrystal.h> 

// define the LCD screen
LiquidCrystal lcd(11, 12, 13, A0, A1, A2);

/**
 * A constant holding the number of the pin associated with the first motor signal of Motor A.
 */
const uint8_t MOTOR_A1 = 6;

/**
 * A constant holding the number of the pin associated with the second motor signal of Motor A.
 */
const uint8_t MOTOR_A2 = 7;

/**
 * A constant holding the number of the pin associated with the first motor signal of Motor B.
 */
const uint8_t MOTOR_B1 = 8;

/**
 * A constant holding the number of the pin associated with the second motor signal of Motor B.
 */
const uint8_t MOTOR_B2 = 9;

/**
 * A constant holding the maximum value to write onto a signal. Equivalent to a logic 1 (HIGH).
 */
const uint8_t MAX_SPEED = 255;

/**
 * A constant holding the minimum value to write onto a signal. Equivalent to a logic 0 (LOW).
 */
const uint8_t MIN_SPEED = 0;


/**
 * An array field holding the motors needed to control motor B 
 */
uint8_t motorB[2];

/**
 * An array field holding the motors needed to control motor A
 */
uint8_t motorA[2];

int16_t baseValue;

boolean initialized;

unsigned int counter;

unsigned int short_delay_counter;

unsigned int lastTick;

int32_t heading_int;

int16_t heading_degree;

int16_t turnRate;

// initialization
void setup()
{
   // LCD has 4 lines with 20 chars
   lcd.begin(20, 4); 
   lcd.print("system ready");

   counter = millis();
   short_delay_counter = millis();
   lastTick = millis();

   initialized = false;
   baseValue = 0;
   heading_int = 0;
   turnRate = 0;

    motorA[0] = MOTOR_A1;
  motorA[1] = MOTOR_A2;

  motorB[0] = MOTOR_B2;
  motorB[1] = MOTOR_B1;

  
  // Set the individual Motor-Signals to behave as outputs.
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);

}

void loop()
{  
   
   // read the current analog value on a3
   int16_t analogValue = analogRead(A3);
  
   if(millis() - short_delay_counter > 100 && !initialized){
    baseValue = analogValue;
    initialized = true;
    
   }
   
   if(baseValue == 0){
    return;
    
   }

   turnRate = analogValue - baseValue;

   if(!(turnRate > -8 && turnRate < 8)){
    heading_int = heading_int + (turnRate * (millis() - lastTick));

    heading_degree = (abs(heading_int) / 1666) % 360;
   }

  if(millis() - counter > 200){
    lcdLogic(analogValue);
    counter = millis();
   }

   lastTick = millis();

   //driveCurve(100, 45);
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
  lcd.print("Analog 3: " + String(mAnalogValue));

  lcd.setCursor(0, 1);

  lcd.print("turn-rate: " + String(turnRate));

  lcd.setCursor(0, 2);

  lcd.print("heading: " + String(heading_int));

  lcd.setCursor(0, 3);

  lcd.print("heading: " + String(heading_degree) + "deg");
}

void driveCurve(const uint8_t mSpeed, const int16_t mCurveRadius){
  
  // method is undefined for curve radix out of the bounds [-90, 90]
  if(mCurveRadius > 90 || mCurveRadius < -90){
    return;
    
  }

  // the factor at which the speed has to adjusted.
  double factor = 1;

  // a flag determining whether to drive the adjusted motor forward or backward.
  boolean forward = true;

  //calculation for the factor at which the speed has to be adjusted.
  if(abs(mCurveRadius) <= 45){
    factor = ((float) (45 - abs(mCurveRadius)))/ (float) (45);
    forward = true;
    
  }else if(abs(mCurveRadius) > 45){
    uint8_t tempRadius = abs(mCurveRadius) % 45;
  
    factor = (45 - tempRadius) / 45;
    forward = false;
    
  }

  // the adjusted Speed. The speed given multiplicated by the factor for the adjustment.
  double adjustedSpeed = factor * mSpeed;
  
  //Motor B is full speed, and Motor A has to adjust to the curve radius
  if(mCurveRadius <= 0){
    setMotorSpeed(true, mSpeed, motorB);

    setMotorSpeed(forward, adjustedSpeed, motorA);

  //Motor A is full speed, and Motor A has to adjust to the curve radius   
  }else if(mCurveRadius > 0){
    setMotorSpeed(true, mSpeed, motorA);

    setMotorSpeed(forward, adjustedSpeed, motorB);
    
  }
}

void setMotorSpeed(const boolean mForward, uint8_t mSpeed, const uint8_t mMotors[]){
  
  // make sure the given speed is in the bounds of [MIN_SPEED, MAX_SPEED]
  if(mSpeed < MIN_SPEED){
    mSpeed = MIN_SPEED;
    
  }else if(mSpeed > MAX_SPEED){
    mSpeed = MAX_SPEED;
    
  }
  
  if(mForward){
    digitalWrite(mMotors[1], LOW);
    analogWrite(mMotors[0], mSpeed);
    return;
    
  }
  analogWrite(mMotors[1], mSpeed);
  digitalWrite(mMotors[0], LOW);
  
}

/* Usefull LCD functions:
set the current write position of the lcd to specific line and row:
  lcd.setCursor(row, line);  

write onto lcd, starting at current position:
  lcd.print("abc");
*/

