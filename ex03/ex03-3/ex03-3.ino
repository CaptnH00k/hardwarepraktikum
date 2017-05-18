/*******************
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);      // open the serial port at 9600 bps:
}

void loop() {
  // put your main code here, to run repeatedly:
  testC();
  testAsm();
  delay(5000);
}

void testC() {
  Serial.println("TEST C, LETS A GO!");
  unsigned long start = micros();
  for(unsigned long i = 0;i < 100000;i++) {
   //Serial.println(String(i)); //make sure we reach the 100.000! uncomment this if you want valid test results
    setPin12(true);
    setPin12(false);
  }
  unsigned long end = micros();

  Serial.println("START: " + String(start));
  Serial.println("END: " + String(end));
  Serial.println("DIFF: " + String(end - start));
  
}

void testAsm() {
  Serial.println("TEST ASM, LETS A GO!");
  unsigned long start = micros();
  for(unsigned long i = 0;i < 100000;i++) {
    //Serial.println(String(i)); //make sure we reach the 100.000! uncomment this if you want valid test results
    setPin12Asm(true);
    setPin12Asm(false);
  }
  unsigned long end = micros();

  Serial.println("START: " + String(start));
  Serial.println("END: " + String(end));
  Serial.println("DIFF: " + String(end - start));
  
}


void setPin12(boolean high) {
  if(high == true) {
    PORTB |= (1 << 4);
  } else {
    PORTB &= ~(1 << 4);  
  }
}

void setPin12Asm(boolean high) {
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

