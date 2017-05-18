/**
 * For a given boolean value True or False set pin 12 to high or low by writing directly in the register. The function "digitalWrite()"
 * must not be used.
 */
void setup() {
  // No setup needed.
}

void loop() {
  // Test code for led toggling. 
  setPin12(true);
  delay(1000);
  setPin12(false);
  delay(1000);
}

void setPin12(boolean high) {
  // Set the 4th bit according to the given boolean value.
  if(high == true) {
    PORTB |= (1 << 4);
  } else {
    PORTB &= ~(1 << 4);  
  }
}
