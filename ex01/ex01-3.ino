void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT); // red
  pinMode(11, OUTPUT); // yellow
  pinMode(12, OUTPUT); // green
}

void loop() {
  // put your main code here, to run repeatedly:

  // turn on the red LED for 5 seconds (don't turn it off)
  digitalWrite(10, HIGH);   
  delay(5000);            

  // turn on the yellow LED (red is still on for 1 second) and turn both off after 1 second
  digitalWrite(11, HIGH); 
  delay(1000);            
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);  

  // turn on the green LED for 3 seconds 
  digitalWrite(12, HIGH);   
  delay(3000);              
  digitalWrite(12, LOW);    

  // turn on the yellow LED for 1 second  
  digitalWrite(11, HIGH);   
  delay(1000);                       
  digitalWrite(11, LOW);

}
