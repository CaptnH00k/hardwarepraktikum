void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // Test code for led toggling. 
  setPin12(true);
  delay(1000);
  setPin12(false);
  delay(1000);
}

void setPin12(boolean high) {
  if(high == true) {
    PORTB |= (1 << 4);
  } else {
    PORTB &= ~(1 << 4);  
  }
}

