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
 * A constant holding the number of the pin associated with the first motor signal.
 */
const uint8_t MOTOR_A1 = 10;

/**
 * A constant holding the number of the pin associated with the second motor signal.
 */
const uint8_t MOTOR_A2 = 11;

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

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup(){
  // Set the individual Motor-Signals to behave as outputs.
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-2#setMotorSpeed(boolean)
 * @see ex02-2#stopMotor()
 */
void loop(){
  stopMotor();

  // TODO: add code for debugging purposes, no actual code implementation of setMotorSpeed is
  // requested from exercise 2

  delay(WAIT_PER_CYCLE_MS);
}
 
/**
 * Makes the motor stop, by writing a logic 0 to both motor signals as specified in
 * {@link ex02-2#MOTOR_A1} and {@link ex02-2#MOTOR_A2}.
 */
void stopMotor(){
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  
}

/**
 * Controls the motor signals specified in {@link ex02-2#MOTOR_A1} 
 * and {@link ex02-2#MOTOR_A2}. If the given boolean is true
 * the motor will move forward, otherwise backwards.
 * 
 * @param mForward
 *            A boolean determining the direction the motor will move to.
 *           
 * @param mSpeed
 *            The speed at which the motor should move, ranging from 0 to 255, 
 *            where 0 is no speed, and 255 is full speed.
 */
void setMotorSpeed(const boolean mForward, uint8_t mSpeed){
  
  // make sure the given speed is in the bounds of [MIN_SPEED, MAX_SPEED]
  if(mSpeed < MIN_SPEED){
    mSpeed = MIN_SPEED;
    
  }else if(mSpeed > MAX_SPEED){
    mSpeed = MAX_SPEED;
    
  }
  
  if(mForward){
    digitalWrite(MOTOR_A1, LOW);
    analogWrite(MOTOR_A2, mSpeed);
    return;
    
  }
  analogWrite(MOTOR_A1, mSpeed);
  digitalWrite(MOTOR_A2, LOW);
  
}

