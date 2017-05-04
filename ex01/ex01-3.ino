/*
 * This program is able to control up to three different 
 * LEDs, and make them flash in different combinations for
 * different amounts of time.
 */

/*
 * A constant holding the number of the pin associated 
 * with the red coloured LED.
 */
const int RED_LED = 10;

/*
 * A constant holding the number of the pin associated
 * with the yellow coloured LED.
 */
const int YELLOW_LED = 11;

/*
 * A constant holding the number of the ping associated
 * with the green coloured LED.
 */
const int GREEN_LED = 12;

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
 * This method gets called once, before the update cycle starts ticking.
 */
void setup() {
  // Make the Pins associated with the LEDS behave as output.
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex01-3#ledCycle()
 */
void loop() {
  ledCycle();
}

/*
 * The logic needed to control three LEDs to blink in a given pattern explained
 * in the 3rd exercise of the first sheet.
 */
void ledCycle() {

  // TODO: error handling, e.g. RED_LED_TIME_ON < YELLOW_LED_TIME_ON, 
  // error for negative integer value in delay(unsigned long)
  
  // First of all start turning the red LED on for its own time - the yellow LEDs time, so both
  // the yellow and the red LED get turned off simultaneously
  digitalWrite(RED_LED, HIGH);   
  delay(RED_LED_ON_TIME - YELLOW_LED_ON_TIME);            

  // Turn on the yellow LED for its respective time
  digitalWrite(YELLOW_LED, HIGH); 
  delay(YELLOW_LED_ON_TIME);

  // Each LED stated here was on for its respective time, thus turn them off
  digitalWrite(RED_LED, LOW);          
  digitalWrite(YELLOW_LED, LOW);

  // Turn on the green LED for its respective time
  digitalWrite(GREEN_LED, HIGH);   
  delay(GREEN_LED_ON_TIME);

  // Turn the green LED off
  digitalWrite(GREEN_LED, LOW);    

  // Turn on the yellow LED for its respective time
  digitalWrite(YELLOW_LED, HIGH);   
  delay(YELLOW_LED_ON_TIME);     

  // Turn the yellow LED off
  digitalWrite(YELLOW_LED, LOW);

}

