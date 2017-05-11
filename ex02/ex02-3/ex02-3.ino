/*
 * In this exercise a function should be implemented which is 
 * able to control the motors of the of the robot, said method 
 * should also be able to control the speed at which the motors move.
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

  motorB[0] = MOTOR_B2;
  motorB[1] = MOTOR_B1;
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-3#driveForward(int, uint8_t)
 * @see ex02-3#stopMotor()
 */
void loop(){
  stopMotor();

  driveForward(1000, 125);

  delay(WAIT_PER_CYCLE_MS);
}
 
/**
 * Makes the motor stop, by writing a logic 0 to both motor signals as specified in
 * {@link ex02-3#motorA} and {@link ex02-3#motorB}.
 */
void stopMotor(){
  setMotorSpeed(true, 0, motorA);
  setMotorSpeed(true, 0, motorB);
  
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
 * Controls the motor signals specified in {@link ex02-3#MOTOR_A1} 
 * and {@link ex02-3#MOTOR_A2}. If the given boolean is true
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
