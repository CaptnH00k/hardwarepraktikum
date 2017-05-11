/*
 * In this exercise a functions should be implemented, so the robot
 * can drive specified patterns.
 * 
 * Every motor has 2 Motor Signals. The movement for a given Motor A 
 * is stated as follows:
 * 
 * A1 = 0, A2 = 0 => STOP
 * A1 = 0, A2 = 1 => FORWARD
 * A1 = 1, A2 = 0 => BACKWARD
 * 
 * 
 * @author Ativelox {@literal ativelox.dev@web.de}
 * 
 */
 
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
 * A constant holding the number of the pin associated with the first motor signal of Motor A.
 */
const uint8_t MOTOR_A1 = 10;

/**
 * A constant holding the number of the pin associated with the second motor signal of Motor A.
 */
const uint8_t MOTOR_A2 = 11;

/**
 * A constant holding the number of the pin associated with the first motor signal of Motor B.
 */
const uint8_t MOTOR_B1 = 6;

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
 * The wait per cycle in ms.
 */
const uint8_t WAIT_PER_CYCLE_MS = 100;

/**
 * Holding the integer value associated with the first button.
 */
const uint8_t BUTTON_1 = 1;

/**
 * Holding the integer value associated with the second button.
 */
const uint8_t BUTTON_2 = 2;

/**
 * Holding the integer value associated with the third button.
 */
const uint8_t BUTTON_3 = 3;

/**
 * Holding the integer value associated with the fourth button.
 */
const uint8_t BUTTON_4 = 4;

/**
 * Holding the integer value associated with the fifth button.
 */
const uint8_t BUTTON_5 = 5;

/**
 * Holds the time needed to drive the side of an rectangle.
 */
const uint16_t TIME_PER_RECTANGLE_SIDE = 2000;

/**
 * Holds the time needed to turn at the edge of an rectangle.
 */
const uint16_t RECTANGLE_TURN_TIME = 1000;

/**
 * Holds the time needed to drive the side of an hexagon.
 */
const uint16_t TIME_PER_HEXAGON_SIDE = 1000;

/**
 * Holds the time needed to turn at the edge of an hexagon.
 */
const uint16_t HEXAGON_TURN_TIME = 500;

/**
 * Holds the time needed to drive the circle.
 */
const uint16_t CIRCLE_DRIVE_TIME = 10000;

/**
 * Holds the time needed to drive the side of an triangle.
 */
const uint16_t TIME_PER_TRIANGLE_SIDE = 2000;

/**
 * Holds the time needed to turn at the edge of an triangle.
 */
const uint16_t TRIANGLE_TURN_TIME = 1000;

/**
 * Assigns at the index [BUTTON] the corresponding pattern boolean.
 */
boolean patterns[6];

/**
 * An array field holding the motors needed to control motor B 
 */
uint8_t motorB[2];

/**
 * An array field holding the motors needed to control motor A
 */
uint8_t motorA[2];

/**
 * Determines whether the circle pattern is currently on or not
 */
boolean isCirclePattern;

/**
 * Determines whether the triangle pattern is currently on or not
 */
boolean isTrianglePattern;

/**
 * Determines whether the hexagon pattern is currently on or not
 */
boolean isHexagonPattern;

/**
 * Determines whether the rectangle pattern is currently on or not
 */
boolean isRectanglePattern;

/**
 * holds the current time before a pattern started.
 */
unsigned long time_before_pattern_start = 0;


/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup(){
  //initialize fields.
  motorA[0] = MOTOR_A1;
  motorA[1] = MOTOR_A2;

  motorB[0] = MOTOR_B2;
  motorB[1] = MOTOR_B1;

  
  isCirclePattern = false;

  isTrianglePattern = false;

  isHexagonPattern = false;

  isRectanglePattern = false;

  // assuming that button 3 is the central button on the arduino board.
  // change here if thats not the case.
  patterns[BUTTON_1] = isCirclePattern;
  patterns[BUTTON_2] = isTrianglePattern;
  patterns[BUTTON_4] = isHexagonPattern;
  patterns[BUTTON_5] = isRectanglePattern;

  
  // Set the individual Motor-Signals to behave as outputs.
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-5#motorPatternLogic(int16_t)
 */
void loop(){
  //stop the movement of the robot at the beginning of each loop.
  stopMotor();

  // read the value from the analog pin A0.
  int16_t analogValue = analogRead(A0);

  // start the main logic, handling patterns and the like.
  motorPatternLogic(analogValue);

  delay(WAIT_PER_CYCLE_MS);
}

/*
 * Gets the current integer value associated with the button wanted.
 * Does so by extracting the button out of the analog value given.
 * Those values aren't constant, thus thresholds are used to
 * identify the appropriate button. Returns 0 if no buttons was pressed.
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
int getCurrentButton(const int16_t mAnalogValue) {
  
  if(mAnalogValue < FIRST_BUTTON_THRESHOLD) {
    return BUTTON_1;
    
  }else if(mAnalogValue < SECOND_BUTTON_THRESHOLD) {
    return BUTTON_2;
    
  }else if(mAnalogValue < THIRD_BUTTON_THRESHOLD) {
    return BUTTON_3;
    
  }else if(mAnalogValue < FOURTH_BUTTON_THRESHOLD) {
    return BUTTON_4;
    
  }else if(mAnalogValue < FIFTH_BUTTON_THRESHOLD) {
    return BUTTON_5;

  // no button pressed
  }else {
    return 0;
    
  }
}

/**
 * Gets the current button press and sets the flag of 
 * the pattern if the corresponding button got pressed.
 * 
 * @param mAnalogValue
 *            The value needed to determine the button.
 * 
 */
void checkButtonPress(const int16_t mAnalogValue){
  patterns[getCurrentButton(mAnalogValue)] = true;
  
}

/**
 * Controls the logic needed to handle different patterns. Only a new pattern
 * can start if the running one completed.
 * 
 * @param mAnalogValue
 *              The value needed to determine the button.
 * 
 */
void motorPatternLogic(const int16_t mAnalogValue){

  //if no pattern is currently driven by the robot
  if(!patterns[BUTTON_1] && !patterns[BUTTON_2] && !patterns[BUTTON_4] && !patterns[BUTTON_5]){
    time_before_pattern_start = millis();
    checkButtonPress(mAnalogValue);
    
    return;
    
  }

  if(patterns[BUTTON_1]){
    driveCirclePattern();
    
  } else if(patterns[BUTTON_2]){
    driveTrianglePattern();
    
  } else if(patterns[BUTTON_4]){
    driveHexagonPattern();
    
  } else if(patterns[BUTTON_5]){
    driveRectanglePattern();
    
  }
}

/**
 * Makes the robot drive in a circular pattern, stops after one circle is completed.
 * Does not block other logic.
 */
void driveCirclePattern(){
  // the time passed since this pattern started
  unsigned long time_since_pattern_start = millis() - time_before_pattern_start;

  //drive in a circle for {@link ex02-5#CIRCLE_DRIVE_TIME} milliseconds.
  if(time_since_pattern_start < CIRCLE_DRIVE_TIME){
    driveCurve(MAX_SPEED / 3, 30);
  
  //circle driving is finished, reset timer, set the flag of the pattern to false and stop the motor.
  }else{
    time_before_pattern_start = millis();
    patterns[BUTTON_1] = false;
    stopMotor();
  }
  
}

/**
 * Makes the robot drive in a triangle pattern, stops after one triangle is completed.
 * Does not block other logic.
 */
void driveTrianglePattern(){
  // the time passed since this pattern started
  unsigned long time_since_pattern_start = millis() - time_before_pattern_start;

  //from here on out calculation on what action to do, so the triangle shape can be achieved.
  if(time_since_pattern_start < TIME_PER_TRIANGLE_SIDE){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < TIME_PER_TRIANGLE_SIDE + TRIANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 2 * TIME_PER_TRIANGLE_SIDE + TRIANGLE_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 2 * TIME_PER_TRIANGLE_SIDE + 2 * TRIANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 3 * TIME_PER_TRIANGLE_SIDE + 2 * TRIANGLE_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 3 * TIME_PER_TRIANGLE_SIDE + 3 * TRIANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);

  //triangle driving is finished, reset timer, set the flag of the pattern to false and stop the motor.
  }else{
    time_before_pattern_start = millis();
    patterns[BUTTON_2] = false;
    stopMotor();
  }
}

/**
 * Makes the robot drive in a hexagonal pattern, stops after one hexagon is completed.
 * Does not block other logic.
 */
void driveHexagonPattern(){
  // the time passed since this pattern started
  unsigned long time_since_pattern_start = millis() - time_before_pattern_start;

  //from here on out calculation on what action to do, so the hexagon shape can be achieved.
  if(time_since_pattern_start < TIME_PER_HEXAGON_SIDE){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < TIME_PER_HEXAGON_SIDE + HEXAGON_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 2 * TIME_PER_HEXAGON_SIDE + HEXAGON_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 2 * TIME_PER_HEXAGON_SIDE + 2 * HEXAGON_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 3 * TIME_PER_HEXAGON_SIDE + 2 * HEXAGON_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 3 * TIME_PER_HEXAGON_SIDE + 3 * HEXAGON_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 4 * TIME_PER_HEXAGON_SIDE + 3 * HEXAGON_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 4 * TIME_PER_HEXAGON_SIDE + 4 * HEXAGON_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);

  }else if(time_since_pattern_start < 5 * TIME_PER_HEXAGON_SIDE + 4 * HEXAGON_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 5 * TIME_PER_HEXAGON_SIDE + 5 * HEXAGON_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 6 * TIME_PER_HEXAGON_SIDE + 5 * HEXAGON_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 6 * TIME_PER_HEXAGON_SIDE + 6 * HEXAGON_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);

  //hexagon driving is finished, reset timer, set the flag of the pattern to false and stop the motor.
  }else{
    time_before_pattern_start = millis();
    patterns[BUTTON_4] = false;
    stopMotor();
  }

}

/**
 * Makes the robot drive in a rectangle pattern, stops after one rectangle is completed.
 * Does not block other logic.
 * 
 */
void driveRectanglePattern(){
  // the time passed since this pattern started
  unsigned long time_since_pattern_start = millis() - time_before_pattern_start;

  //from here on out calculation on what action to do, so the rectangle shape can be achieved.
  if(time_since_pattern_start < TIME_PER_RECTANGLE_SIDE){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < TIME_PER_RECTANGLE_SIDE + RECTANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 2 * TIME_PER_RECTANGLE_SIDE + RECTANGLE_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 2 * TIME_PER_RECTANGLE_SIDE + 2 * RECTANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 3 * TIME_PER_RECTANGLE_SIDE + 2 * RECTANGLE_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 3 * TIME_PER_RECTANGLE_SIDE + 3 * RECTANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);
    
  }else if(time_since_pattern_start < 4 * TIME_PER_RECTANGLE_SIDE + 3 * RECTANGLE_TURN_TIME){
    driveForward(MAX_SPEED / 3);
    
  }else if(time_since_pattern_start < 4 * TIME_PER_RECTANGLE_SIDE + 4 * RECTANGLE_TURN_TIME){
    driveCurve(MAX_SPEED / 3, 90);

  //rectangle driving is finished, reset timer, set the flag of the pattern to false and stop the motor.
  }else{
    time_before_pattern_start = millis();
    patterns[BUTTON_5] = false;
    stopMotor();
  }

  
  
}
 
/**
 * Makes the motor stop, by writing a logic 0 to both motor signals as specified in
 * {@link ex02-5#MOTOR_A1} and {@link ex02-5#MOTOR_A2}.
 */
void stopMotor(){
  setMotorSpeed(true, 0, motorA);
  setMotorSpeed(true, 0, motorB);
  
}

/**
 * Controls both motors so the robot will move straight forward.
 *          
 * @param mSpeed
 *          An 8-Bit integer value determining at which speed to drive
 *          forward, ranging from 0 to 255.
 * 
 */
void driveForward(const uint8_t mSpeed){
  
  // drive both motors forward e.g. motor A and motor B.
  setMotorSpeed(true, mSpeed, motorA);
  setMotorSpeed(true, mSpeed, motorB);
  
}

/**
 * Drives a curve and adjusts the motor strength by the curve radius given, ranging
 * from -90 to 90. Where 90 radius means the robot will turn on the spot, 0 means the
 * robot will move forward and -90 means the robot will turn in the other direction on the spot.
 *          
 * @param mSpeed
 *          The speed at which the Motors will move, taking the curve radius
 *          into consideration
 *          
 * @param mCurveRadius
 *          The radius at which the motors will move. (e.g. the strength of the curve)
 */
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


/**
 * Controls the motor signals specified in {@link ex02-5#MOTOR_A1} 
 * and {@link ex02-5#MOTOR_A2}. If the given boolean is true
 * the motor will move forward, otherwise backwards.
 * 
 * @param mForward
 *            A boolean determining the direction the motor will move to.
 *           
 * @param mSpeed
 *            The speed at which the motor should move, ranging from 0 to 255, 
 *            where 0 is no speed, and 255 is full speed.
 *            
 * @param mMotors
 *            An 8-Bit integer array containing the two motor signals needed to control one motor.
 *            
 */
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
