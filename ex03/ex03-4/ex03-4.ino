const uint8_t SPEAKER_PIN = 12;

boolean on = false;

void setup() {
  Serial.begin(9600);
  
  setTimerInterruptionEvent(SPEAKER_PIN, 1200, 3);
  
}

void loop(){
}

void setTimerInterruptionEvent(const uint8_t mPin, unsigned long mFrequency, int mBytePrescaleShift){
  

  if(mFrequency < 100 || mFrequency > 2000){
    return;
    
  }

  if(mBytePrescaleShift > 4) {
    return;
  }
  
  unsigned long coreFrequencyHz = 8000000;
  unsigned long runsPerSecond = mFrequency * 2;
  unsigned long counterFreq = coreFrequencyHz / mBytePrescaleShift;
  unsigned long counterOverflowReset =  counterFreq / runsPerSecond;

  // pin as output
  pinMode(mPin, OUTPUT);

  cli(); 

  // reset control registers
  TCCR2A = 0;

  TCCR2B = 0;  

  Serial.println(String(CS22));

  if(mBytePrescaleShift == 1) {
    TCCR2B |= (1 << CS20); 
  } else if(mBytePrescaleShift == 2) {
    TCCR2B |= (1 << CS21);
  } else if(mBytePrescaleShift == 3) {
    TCCR2B |= (1 << CS20);
    TCCR2B |= (1 << CS21);
  } else if(mBytePrescaleShift == 4) {
    TCCR2B |= (1 << CS22);
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
