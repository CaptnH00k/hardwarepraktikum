/*
 * In this exercise a function should be implemented which controls
 * the motors in a way that the robot is able to drive curves.
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
 * An array field holding the motors needed to control motor B 
 */
uint8_t motorB[2];

/**
 * An array field holding the motors needed to control motor A
 */
uint8_t motorA[2];

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup(){
  
  // Set the individual Motor-Signals to behave as outputs.
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);

  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);

  //initialize arrays.
  motorA[0] = MOTOR_A1;
  motorA[1] = MOTOR_A2;

  //because for motor b the signals are
  //reverse, we also reverse the pins. 
  motorB[0] = MOTOR_B2;
  motorB[1] = MOTOR_B1;
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-4#driveCurve(int, uint_8, uint_16)
 * @see ex02-4#stopMotor()
 */
void loop(){
  stopMotor();

  // TODO: add code for debugging purposes, no actual code implementation of setMotorSpeed is
  // requested from exercise 4

  driveCurve(5000, 100, 10);

  delay(WAIT_PER_CYCLE_MS);
}
 
/**
 * Makes the motor stop, by writing a logic 0 to both motor signals as specified in
 * {@link ex02-4#MOTOR_A1} and {@link ex02-4#MOTOR_A2}.
 */
void stopMotor(){
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  
}

/**
 * Controls both motors so the robot will move straight forward.
 * 
 * @param mTime
 *          An integer value determining how long to drive forward.
 *          
 * @param mSpeed
 *          An 8-Bit integer value determining at which speed to drive
 *          forward, ranging from 0 to 255.
 * 
 */
void driveForward(const int mTime, const uint8_t mSpeed){
  
  // drive both motors forward e.g. motor A and motor B.
  setMotorSpeed(true, mSpeed, motorA);
  setMotorSpeed(true, mSpeed, motorB);

  // drive forward for the time given.
  delay(mTime);
  
}

/**
 * Drives a curve and adjusts the motor strength by the curve radius given, ranging
 * from -90 to 90. Where 90 radius means the robot will turn on the spot, 0 means the
 * robot will move forward and -90 means the robot will turn in the other direction on the spot.
 * 
 * @param mTime
 *          The robot will drive as long as the time specified.
 *          
 * @param mSpeed
 *          The speed at which the Motors will move, taking the curve radius
 *          into consideration
 *          
 * @param mCurveRadius
 *          The radius at which the motors will move. (e.g. the strength of the curve)
 */
void driveCurve(const int mTime, const uint8_t mSpeed, const int16_t mCurveRadius){
  
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

  delay(mTime);
}


/**
 * Controls the motor signals specified in {@link ex02-4#MOTOR_A1} 
 * and {@link ex02-4#MOTOR_A2}. If the given boolean is true
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
