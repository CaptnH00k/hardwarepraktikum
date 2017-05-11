/*
 * In this exercise a function should be implemented which is 
 * able to control the motors of the of the robot. Basic controlling, 
 * e.g. forward and backward movement.
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
 * @see ex02-1#setMotorSpeed(boolean)
 * @see ex02-1#stopMotor()
 */
void loop(){
  stopMotor();

  setMotorSpeed(true);

  delay(WAIT_PER_CYCLE_MS);
}
 
/**
 * Makes the motor stop, by writing a logic 0 to both motor signals as specified in
 * {@link ex02-1#MOTOR_A1} and {@link ex01-1#MOTOR_A2}.
 */
void stopMotor(){
  digitalWrite(MOTOR_A1, LOW);
  digitalWrite(MOTOR_A2, LOW);
  
}

/**
 * Controls the motor signals specified in {@link ex02-1#MOTOR_A1} 
 * and {@link ex01-1#MOTOR_A2}. If the given boolean is true
 * the motor will move forward, otherwise backwards.
 * 
 * @param mForward
 *            A boolean determining the direction the motor will move to.
 */
void setMotorSpeed(const boolean mForward){
  if(mForward){
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_A1, HIGH);
    return;
    
  }
  digitalWrite(MOTOR_A2, HIGH);
  digitalWrite(MOTOR_A1, LOW);
  
}

