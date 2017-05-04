// LED - test : Blinking with 1Hz frequency
// LEDs are connected to Pin 10 and 11

// global variables
int ledPin1 = 10;
int ledPin2 = 11;

// initialization
void setup(){
  // set ledPins as output
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}


void loop(){
  // turn on LED 1 for 0.5 seconds and wait for 0.5 seconds
  digitalWrite(ledPin1, HIGH);
  delay(500);
  digitalWrite(ledPin1, LOW);
  delay(500);
  
  // turn on LED 2 for 0.5 seconds and wait for 0.5 seconds
  digitalWrite(ledPin2, HIGH);
  delay(500);
  digitalWrite(ledPin2, LOW);
  delay(500);
}
