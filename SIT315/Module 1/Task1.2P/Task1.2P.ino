// Define the input and output pins
int inputPinMotion = 2;       // Input Pin for motion sensor
int LED_MOTION = 7;           // Output Pin for motion LED


// Variables to track the state of motion and sound detection
volatile int pirState = LOW;  // Initialize motion state as low 

// Variables to store the current readings from motion and sound sensors
int motionVal = 0;             // Variable for reading the pin status of motion sensor


void setup() {
  // Set pin modes for motion sensor and sound sensor with internal pull-up resistors
  pinMode(inputPinMotion, INPUT_PULLUP);

  // Set pin modes for LED outputs
  pinMode(LED_MOTION, OUTPUT);

  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Attach interrupts to the input pins to call corresponding functions when a CHANGE is detected
  attachInterrupt(digitalPinToInterrupt(inputPinMotion), motionDetected, CHANGE); // Motion sensor interrupt
}

void loop() {

}


void motionDetected() {
  motionVal = digitalRead(inputPinMotion);  // Read input value from motion sensor
  if (motionVal == HIGH) {                  // Check if the input is HIGH
    digitalWrite(LED_MOTION, HIGH);         // Turn motion LED ON
    if (pirState == LOW) {
      // Print Motion Detected only when the state changes from LOW to HIGH (not continuously)
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
  } else {
    digitalWrite(LED_MOTION, LOW);          // Turn motion LED OFF
    if (pirState == HIGH){
      // Print Motion Ended only when the state changes from HIGH to LOW (not continuously)
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}

