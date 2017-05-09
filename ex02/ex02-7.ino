/*
 * In this exercise a functions should be implemented, so the robot
 * can drive without crashing into obstacles, basically automatically
 * avoiding every obstacle if BUTTON_3 is pressed.
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
const uint8_t BUTTON_2 = 1;

/**
 * Holding the integer value associated with the third button.
 */
const uint8_t BUTTON_3 = 1;

/**
 * Holding the integer value associated with the fourth button.
 */
const uint8_t BUTTON_4 = 1;

/**
 * Holding the integer value associated with the fifth button.
 */
const uint8_t BUTTON_5 = 1;

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

/**
 * Holds the value of when to turn if the distance is below this threshold (in centimeters)
 */
const uint8_t DISTANCE_THRESHOLD = 30;

/**
 * An array field holding the motors needed to control motor B 
 */
uint8_t motorB[2];

/**
 * An array field holding the motors needed to control motor A
 */
uint8_t motorA[2];

/**
 * Determines whether the robot is driving or not.
 */
boolean isDriving;


/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup(){
  //initialize fields.

  isDriving = false;
  
  motorA[0] = MOTOR_A1;
  motorA[1] = MOTOR_A2;

  motorB[0] = MOTOR_B1;
  motorB[1] = MOTOR_B2;

  
  // Set the individual Motor-Signals to behave as outputs.
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);

  
  //set the ultrasonic sensor up, so it can measure distances.
  pinMode(ULTRASONIC_SENSOR, OUTPUT);

  impulse(ULTRASONIC_SENSOR);
  
  pinMode(ULTRASONIC_SENSOR, INPUT);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-7#drivingLogic(int16_t)
 */
void loop(){
  //stop the movement of the robot at the beginning of each loop.
  stopMotor();

  // read the value from the analog pin A0.
  int16_t analogValue = analogRead(A0);

  // start the main logic, handling the driving of the robot.
  drivingLogic(analogValue);

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
 * If the pressed button was BUTTON_3, then start driving, by setting the flag.
 * 
 * @param mAnalogValue
 *            The value needed to determine the button.
 * 
 */
void checkButtonPress(const int16_t mAnalogValue){
  if(getCurrentButton(mAnalogValue) == BUTTON_3){
    isDriving = !isDriving;
    
  }
}

/**
 * Controls the logic needed to handle driving. By pressing the
 * button 3 the robot will start/stop driving, depending on
 * his current state.
 * 
 * @param mAnalogValue
 *              The value needed to determine the button.
 * 
 */
void drivingLogic(const int16_t mAnalogValue){

  //always check whether the button got pressed, so the robot 
  //can stop driving if wanted to
  checkButtonPress(mAnalogValue);

  //if the robot isn't driving don't drive.
  if(!isDriving){
    return;
   
  }
  startDriving();
  
}

/**
 * Starts the robots driving logic. Avoids obstacles if the distance to 
 * an obstacle is less or equal than {@link ex02-7#DISTANCE_THRESHOLD}
 */
void startDriving(){
  
  //holds the value of the distance to the nearest obstacle 
  //in centimeters, or -1 if there is no obstacle.
  int32_t distance = measureDistance(ULTRASONIC_SENSOR);

  //make sure not to drive curves if theres no obstacle.
  if(distance <= DISTANCE_THRESHOLD && distance >= 0){
    driveCurve(MAX_SPEED, 90);
    
  }else{
    driveForward(MAX_SPEED);
    
  }
}
 
/**
 * Makes the motor stop, by writing a logic 0 to both motor signals as specified in
 * {@link ex02-7#MOTOR_A1} and {@link ex02-7#MOTOR_A2}.
 */
void stopMotor(){
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  
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
  uint8_t factor = 1;

  // a flag determining whether to drive the adjusted motor forward or backward.
  boolean forward = true;

  //calculation for the factor at which the speed has to be adjusted.
  if(abs(mCurveRadius) <= 45){
    factor = (45 - abs(mCurveRadius))/45;
    forward = true;
    
  }else if(abs(mCurveRadius) > 45){
    uint8_t tempRadius = abs(mCurveRadius) % 45;
  
    factor = (45 - tempRadius) / 45;
    forward = false;
    
  }

  // the adjusted Speed. The speed given multiplicated by the factor for the adjustment.
  uint8_t adjustedSpeed = factor * mSpeed;
  
  //Motor B is full speed, and Motor A has to adjust to the curve radius
  if(mCurveRadius < 0){
    setMotorSpeed(true, mSpeed, motorB);

    setMotorSpeed(forward, adjustedSpeed, motorA);

  //Motor A is full speed, and Motor A has to adjust to the curve radius   
  }else if(mCurveRadius > 0){
    setMotorSpeed(true, mSpeed, motorA);

    setMotorSpeed(forward, adjustedSpeed, motorB);
    
  }
}


/**
 * Controls the motor signals specified in {@link ex02-7#MOTOR_A1} 
 * and {@link ex02-7#MOTOR_A2}. If the given boolean is true
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
    digitalWrite(mMotors[0], LOW);
    analogWrite(mMotors[1], mSpeed);
    return;
    
  }
  analogWrite(mMotors[0], mSpeed);
  digitalWrite(mMotors[1], LOW);
  
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
 * in {@link ex02-7#IMPULSE_WAIT_US} from high to low.
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

