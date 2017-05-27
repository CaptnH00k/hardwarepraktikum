/*
 * This class is able to set timer interruptions so it can play 
 * sounds and toggle LEDs at random intervalls.
 */

//holding the value of the pin the speaker is using
const uint8_t SPEAKER_PIN = 12;

//holding whether the speaker pin is HIGH or LOW
boolean on = false;

//holding whether the LED is HIGH or LOW
boolean ledIsOn = false;

//holding 10 random values between 500 and 2500
uint32_t duration[10];

//holding 10 random value between 100 and 2000
uint32_t frequency[10];

//holding the value of the pin the LED is using
const uint8_t LED_PIN = 11;

//holding the value of how many milliseconds have passed since program start.
volatile uint32_t tCount = 0;

//a random value which is based off of tcount and a random number
volatile uint32_t sCount = 0;

//index which gets incremented when sCount > tCount
volatile uint8_t index = 0;

//the values the prescaler can have
const uint32_t prescalers[6] = {8,32,64,128,256,1024};

/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup() {
  Serial.begin(38400);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  for(int i = 0; i < 10; i++){
    duration[i] = random(500, 2501);
    frequency[i] = random(100, 2001);
    
  }
  setTimerOneInterruptionEvent();
  
}

/**
 * Sets a timer interruption event for the second timer. Sends the given PIN
 * in the given frequency HIGHS and LOWS.
 * 
 * @param mPin 
 *          The pin on which to use PWM.
 * @param mFrequency 
 *          The frequency at which to send HIGH and LOW.
 */
void setTimerTwoInterruptionEvent(const uint8_t mPin, unsigned long mFrequency){

  //don't do anything if the frequency isn't in the given bounds.
  if(mFrequency < 100 || mFrequency > 2000){
    return;
    
  }

  //calculate the TOP for the OCR2A
  unsigned long coreFrequencyHz = 8000000;
  unsigned long runsPerSecond = mFrequency * 2;
  unsigned long counterOverflowReset =  coreFrequencyHz / runsPerSecond;
  uint32_t prescale = 1;

  if(counterOverflowReset > 255){

    for(int i = 0; i < 7; i++){
      if((counterOverflowReset / prescalers[i]) <= 255){
        prescale = prescalers[i];
        counterOverflowReset = counterOverflowReset / prescalers[i];
        break;
      }

      if(i == 6){
        return;
      }
    }
    
  }

  cli(); 

  // reset control registers
  TCCR2A = 0;
  TCCR2B = 0;  

  //set the prescale so the value for the OCR2A is in the bounds (0...255)
  if(prescale == 1) {
    TCCR2B |= (1 << CS20); 
  } else if(prescale == 8) {
    TCCR2B |= (1 << CS21);
  } else if(prescale == 32) {
    TCCR2B |= (1 << CS20);
    TCCR2B |= (1 << CS21);
  } else if(prescale == 64) {
    TCCR2B |= (1 << CS22);
  } else if(prescale == 128){
    TCCR2B |= (1 << CS22);
    TCCR2B |= (1 << CS20);
  } else if(prescale == 256){
    TCCR2B |= (1 << CS22);
    TCCR2B |= (1 << CS21);
  }else if(prescale == 1024){
    TCCR2B |= (1 << CS22);
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS20);
  }
  
       
  // set mode (CTC)
  TCCR2A |= (1 << WGM21);
    
  // set output compare register A
  OCR2A = counterOverflowReset;
    
  // enable interrupt
  TIMSK2 |= (1 << OCIE2A);    
  
  // enable all interrupts
  sei();
  
}

/**
 * Sets a timer interruption event for the first timer.
 */
void setTimerOneInterruptionEvent(){

  //disable all interrupts.
  cli();

  //reset registers
  TCCR1A = 0;
  TCCR1B = 0;

  //set the OCR1A register to the correct value to count up once per millisecond.
  OCR1A = 7999;

  // set mode CTC
  TCCR1B |= (1 << WGM12);

  // set the prescale 
  TCCR1B |= (1 << CS10);

  // enable interrupt
  TIMSK1 |= (1 << OCIE1A);

  //enable all interrupts
  sei();
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 */
void loop(){
  Serial.println(String(tCount));
  delay(1000);
  
}

/**
 * Toggle the given LED, if it's ON turn it OFF and vice versa.
 */
void toggleLED(const int mLED){
  if(ledIsOn){
    digitalWrite(mLED, LOW);
    ledIsOn = false;
  }else{
    digitalWrite(mLED, HIGH);
    ledIsOn = true;
  }
}

// interrupt service routine for timer 1 compare match
ISR(TIMER1_COMPA_vect){
  tCount += 1;

  if(tCount >= sCount){
    index = (index + 1) % 10;
    sCount = tCount + duration[index];
    toggleLED(LED_PIN);
    setTimerTwoInterruptionEvent(SPEAKER_PIN, frequency[index]);
    
  }
  
}

// interrupt service routine for timer 2 compare match
ISR(TIMER2_COMPA_vect) {
  // toggle pin
  if(on == false){
   PORTB |= (1 << 4);
    on = true;
  } else {
    PORTB &= ~(1 << 4);
    on = false;
  }
}
