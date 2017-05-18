/**
 * According to a given boolean value set pin 12 to high or low by writing directly in the assembler.
 */
void setup() {
  //  No setup needed.
}

void loop() {
  // Test code for LED toggling. 
  setPin12Asm(true);
  delay(1000);
  setPin12Asm(false);
  delay(1000);
}

void setPin12Asm(boolean high) {
  if(high == true) {
    // Set fourth bit to 1 in register PORTB.
    asm volatile (
    "sbi %0, %1\n\t" // 2 clocks
    :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB4)
    );
  } else {
    // Set fourth bit to zero in register PORTB. 
    asm volatile (
    "cbi %0, %1\n\t" // 2 clocks
    :: "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB4)
    );
  }
}
