const uint8_t SPEAKER_PIN = 12;

boolean on = false;

boolean ledIsOn = false;

uint32_t duration[10];

uint32_t frequency[10];

const uint8_t LED_PIN = 11;

volatile uint32_t tCount = 0;

volatile uint32_t sCount = 0;

volatile uint8_t index = 0;

const uint32_t prescalers[6] = {8,32,64,128,256,1024};

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

void setTimerTwoInterruptionEvent(const uint8_t mPin, unsigned long mFrequency){

  if(mFrequency < 100 || mFrequency > 2000){
    return;
    
  }
  
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

void setTimerOneInterruptionEvent(){

  cli();
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 7999;

  TCCR1B |= (1 << WGM12);

  TCCR1B |= (1 << CS10);

  TIMSK1 |= (1 << OCIE1A);

  sei();
}

void loop(){
  Serial.println(String(tCount));
  delay(1000);
  
}

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
