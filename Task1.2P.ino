// Define the input and output pins
int inputPinMotion = 2;       // Input Pin for motion sensor
int inputPinSound = 3;        // Input Pin for sound sensor
int LED_MOTION = 7;           // Output Pin for motion LED
int LED_SOUND = 8;            // Output Pin for sound LED

// Variables to track the state of motion and sound detection
volatile int pirState = LOW;  // Initialize motion state as low (volatile keyword is used for variables shared with an interrupt routine)
volatile int soundState = LOW; // Initialize sound state as low

// Variables to store the current readings from motion and sound sensors
int motionVal = 0;             // Variable for reading the pin status of motion sensor
int soundVal = 0;              // Variable for reading the pin status of sound sensor

void setup() {
  // Set pin modes for motion sensor and sound sensor with internal pull-up resistors
  pinMode(inputPinMotion, INPUT_PULLUP);
  pinMode(inputPinSound, INPUT_PULLUP);

  // Set pin modes for LED outputs
  pinMode(LED_MOTION, OUTPUT);
  pinMode(LED_SOUND, OUTPUT);

  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Attach interrupts to the input pins to call corresponding functions when a CHANGE is detected
  attachInterrupt(digitalPinToInterrupt(inputPinSound), soundDetected, CHANGE); // Sound sensor interrupt
  attachInterrupt(digitalPinToInterrupt(inputPinMotion), motionDetected, CHANGE); // Motion sensor interrupt
}

void loop() {
  // The main loop will handle other tasks (if any).
  // The interrupt will take care of motion and sound detection.
}

void soundDetected() {
  soundVal = digitalRead(inputPinSound);  // Read input value from sound sensor
  if (soundVal == HIGH) {                // Check if the input is HIGH
    digitalWrite(LED_SOUND, HIGH);       // Turn sound LED ON
    if (soundState == LOW) {
      // Print Sound Detected only when the state changes from LOW to HIGH (not continuously)
      Serial.println("Sound detected!");
      soundState = HIGH;
    }
  } else {
    digitalWrite(LED_SOUND, LOW);        // Turn sound LED OFF
    if (soundState == HIGH){
      // Print Sound Ended only when the state changes from HIGH to LOW (not continuously)
      Serial.println("Sound ended!");
      soundState = LOW;
    }
  }
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

