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
    // Set 4th bit to one in register PORTB 
    asm volatile (
    "sbi %0, %1\n\t" // 2 clocks
    :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB4)
    );
  } else {
    // Set  4th bit to zero in register PORTB 
    asm volatile (
    "cbi %0, %1\n\t" // 2 clocks
    :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB4)
    );
  }
}

