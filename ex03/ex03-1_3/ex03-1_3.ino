/*
 * This class provides low-level methods to set bits in specified registers
 * to send HIGH or LOW values to Pin12. Also provides time measurements,
 * comparing whether C, or Assembler works faster.
 * 
 * Testergebnisse:
 * SERIAL OUTPUT:
 * > TEST C, LETS A GO!
 * > START: 131483040
 * > END: 131609096
 * > DIFF: 126056
 * > TEST ASM, LETS A GO!
 * > START: 131614912
 * > END: 131741648
 * > DIFF: 126736
 * Komischerweise ist unser C code schneller als unser ASM code.
 * Unerklärbar?!
 */
 
/*
 * This method gets called once, before the update cycle starts ticking.
 */
void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps:
  
}

/*
 * The main-loop of this program. Gets repeatedly called without delay.
 * 
 * @see ex02-1_3#testC()
 * @see ex02-1_3#testAsm()
 */
void loop() {
  // put your main code here, to run repeatedly:
  testC();
  testAsm();
  delay(5000);
}

/**
 * Time measurement for testing the C-Code used to set Pin12 to HIGH or Low.
 */
void testC() {
  Serial.println("TEST C, LETS A GO!");
  unsigned long start = micros();
  for(unsigned long i = 0;i < 100000;i++) {
    //make sure we reach the 100.000! uncomment this if you want valid test results
    //Serial.println(String(i)); 
    setPin12(true);
    setPin12(false);
  }
  unsigned long end = micros();

  Serial.println("START: " + String(start));
  Serial.println("END: " + String(end));
  Serial.println("DIFF: " + String(end - start));
  
}

/**
 * Time measurement for testing the Assembler-Code used to set Pin12 to HIGH or LOW.
 */
void testAsm() {
  Serial.println("TEST ASM, LETS A GO!");
  unsigned long start = micros();
  for(unsigned long i = 0;i < 100000;i++) {
    //make sure we reach the 100.000! uncomment this if you want valid test results
    //Serial.println(String(i)); 
    setPin12Asm(true);
    setPin12Asm(false);
  }
  unsigned long end = micros();

  Serial.println("START: " + String(start));
  Serial.println("END: " + String(end));
  Serial.println("DIFF: " + String(end - start));
  
}

/**
 * Sets pin 12 to the given boolean value, where true equals 
 * HIGH and false equals LOW. Sets bits directly in the appropriate
 * register to achieve the desired effect.
 * 
 * @param mHigh
 *          The boolean determining whether the pin gets set
 *          to HIGH or LOW.
 */
void setPin12(const boolean mHigh) {
  if(mHigh == true) {
    PORTB |= (1 << 4);
  } else {
    PORTB &= ~(1 << 4);  
  }
}

/**
 * Sets pin 12 to the given boolean value, where true equals
 * HIGH and false equals LOW. Does so by using assembler
 * and setting bits directly in the appropriate register.
 * 
 * @param mHigh
 *         The boolean determining whether the pin gets set
 *         to HIGH or LOW
 */
void setPin12Asm(const boolean mHigh) {
  if(mHigh == true) {
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

