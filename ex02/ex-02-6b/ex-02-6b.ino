int UC_PIN = 12;

int val = 0;
long millies;
boolean setupUC = false;

void setup() {
  Serial.begin(9600);
}

int measureDistance(int pin) {
    pinMode(pin, OUTPUT);  
    digitalWrite(pin, HIGH);
    delayMicroseconds(11);
    digitalWrite(pin, LOW);  
    pinMode(pin, INPUT);
    setupUC = true;
    Serial.println("Setup complete");
    long timeStart = micros();
    Serial.println("start: " + String(timeStart));
    while(digitalRead(UC_PIN) == 1 && (micros() - timeStart <= 30)) {
      
    }
    long timeEnd = micros();
    Serial.println("end: " + String(timeEnd));
    return (timeEnd - timeStart) / 58;
}

void loop() {
  Serial.println(measureDistance(UC_PIN));
}

