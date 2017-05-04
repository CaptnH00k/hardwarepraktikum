// LED - test : Blinking with 1Hz frequency
// LED is connected to Pin 10

// global variables
int ledPin = 10;

// initialization
void setup(){
  // set ledPin as output
  pinMode(ledPin, OUTPUT);
}


void loop(){
  // enable the LED
  digitalWrite(ledPin, HIGH);
  // wait for 500ms
  delay(500);
  // disable the LED
  digitalWrite(ledPin, LOW);
  // wait for 500ms
  delay(500);
}
