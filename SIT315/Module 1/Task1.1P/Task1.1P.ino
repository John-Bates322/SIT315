int motionPin = 2;               // Input Pin for Motion Sensor
int pirState = LOW;             // Intiliase State default as low
int val = 0;                    // variable for reading the pin status
 
void setup() {    
  pinMode(motionPin, INPUT);     // declare sensor as input
  pinMode(LED_BUILTIN, OUTPUT); // declare LED as output
  Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(motionPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(LED_BUILTIN, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // Print Motion Detected
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW); // turn LED OFF
    if (pirState == HIGH){
      //Print motion ended
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}
