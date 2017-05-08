// LED - test : Blinking with 1Hz frequency
// LEDs are connected to Pin 10 and 11

/*
 * A constant holding the number of the pin for the first LED.
 */
const int LED_PIN_1 = 10;

/*
 * A constant holding the number of the pin for the second LED
 */
const int LED_PIN_2 = 11;

/*
 * A value representating how long to wait before the next event gets 
 * fired in milliseconds.
 */
const int DELAY_TIME = 500;

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup(){
  //Set the Led-Pins as output
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex01-2#ledBlink(int mDelayTime)
 */
void loop(){
  ledBlink(DELAY_TIME);
  
}

/*
 * The logic needed to make two LEDs blink after each other.
 * 
 * A blink is considered a LED which is turned on for the time
 * specified in mDelayTime and turned off for the
 * exact same time.
 * 
 * @param mDelayTime
 *              An integer value determining the time the LED is turned on.
 */
void ledBlink(const int mDelayTime){
  
  // turn on LED 1 for the value of {@link ex01-2#DELAY_TIME} milliseconds.
  digitalWrite(LED_PIN_1, HIGH);
  delay(mDelayTime);

  // turn off led 1 for the value of {@link ex01-2#DELAY_TIME} milliseconds.
  digitalWrite(LED_PIN_1, LOW);
  delay(mDelayTime);
  
  // turn on LED 2 for the value of {@link ex01-2#DELAY_TIME} milliseconds.
  digitalWrite(LED_PIN_2, HIGH);
  delay(mDelayTime);

  // turn off LED 2 for the value of {@link ex01-2#DELAY_TIME} milliseconds.
  digitalWrite(LED_PIN_2, LOW);
  delay(mDelayTime);
  
}

