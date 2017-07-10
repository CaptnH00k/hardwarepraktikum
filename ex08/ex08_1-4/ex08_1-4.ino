// TurnRate Sensor: read data from analog gyroscope, output heading on lcd and send data to the FPGA via UART.

/* Connections:
  R/S : Pin 11
  R/W : not connected
  E   : Pin 12
  DB4 : Pin 13
  DB5 : Pin A0
  DB6 : Pin A1
  DB7 : Pin A2


  Turn-Rate Sensor: A3
*/
// include LCD functions:
#include <LiquidCrystal.h>

// define the LCD screen
LiquidCrystal lcd(11, 12, 13, A0, A1, A2);

// the base value of the turn sensor, e.g. the neutral value.
int16_t baseValue;

// a boolean determining whether the base value has yet been set.
boolean initialized;

// the counter used for the lcd logic cycle.
unsigned int counter;

// the counter used for the data sending to the fpga.
unsigned int sendCounter;

// the counter used to initialize the base value.
unsigned int short_delay_counter;

// the millis since the last tick after calculating the heading values.
unsigned int lastTick;

// an integer holding the "heading" value as described on the sheet.
int32_t heading_int;

// the degrees the board is heading. resets at 360 so it accurately describes them.
int heading_degree;

// the turn rate of the sensor for the current tick.
int16_t turnRate;

// how often to send signals to the fpga in milliseconds.
const unsigned int sendFrequency = 5;

// initialization
void setup()
{
  // LCD has 4 lines with 20 chars
  lcd.begin(20, 4);
  lcd.print("system ready");

  counter = millis();
  short_delay_counter = millis();
  lastTick = millis();
  sendCounter = millis();

  Serial.begin(38400);

  initialized = false;
  baseValue = 0;
  heading_int = 0;
  turnRate = 0;

}

void loop()
{

  // read the current analog value on a3
  int16_t analogValue = analogRead(A3);

  // wait for 100 milliseconds, then read the value from the turn rate sensor and set it as
  //the base value. Only execute this once.
  if (millis() - short_delay_counter > 100 && !initialized) {
    baseValue = analogValue;
    initialized = true;

  }

  // if we don't have a value to calculate from, we just don't do anything
  if (baseValue == 0) {
    return;

  }

  int16_t difference = millis() - lastTick;

  turnRate = analogValue - baseValue;

  lastTick = millis();

  //add a little margin, so the heading won't flunctuate as much. As long as the turn rate is in
  //the bounds -8 < x < 8 the heading won't change.
  if (!(turnRate > -8 && turnRate < 8)) {
    heading_int = heading_int + turnRate * difference;

    //heading_degree = (abs(heading_int) / 3174) % 360;
    heading_degree = (abs(heading_int) / 6348) % 180;
  }

  //update the turn rate on the fpga every millisecond, or 1000 microseconds, since reading from the new data
  //takes about 200 microseconds we just wait for a while to send the next datastring.
  if (millis() - sendCounter > sendFrequency) {
    Serial.write(heading_degree);
    sendCounter = millis();
  }

  //update the lcd every 2 seconds.
  if (millis() - counter > 2000) {
    lcdLogic(analogValue);
    counter = millis();
  }
}

/*
   The logic used to control the LCD. See method body for more information.

   This method completely blocks other logic-implementation for the time
   specified in {@link ex01-6#FLICKER_PREVENTION_TIME}.

   @param mAnalogValue
                An integer value which gets printed onto the LCDs screen.
*/
void lcdLogic(const int mAnalogValue) {

  // Clear display each time this method gets called, so the
  // old LCD-display doesn't get overwritten
  lcd.clear();

  // Sets the cursor to the first row at the first char
  lcd.setCursor(0, 0);

  // Prints the value given onto the LCD
  lcd.print("Analog 3: " + String(mAnalogValue));

  lcd.setCursor(0, 1);

  lcd.print("turn-rate: " + String(turnRate));

  lcd.setCursor(0, 2);

  lcd.print("heading: " + String(heading_int));

  lcd.setCursor(0, 3);

  lcd.print("heading: " + String(heading_degree) + "deg");
}


