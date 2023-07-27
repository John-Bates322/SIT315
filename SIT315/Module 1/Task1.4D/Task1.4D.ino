#include <DHT.h>

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 

// DHT sensor instance
DHT dht(DHTPIN, DHTTYPE);

// Define the input and output pins
const int inputPinMotion = 2;   // Input Pin for motion sensor
const int inputPinSound = 3;    // Input Pin for sound sensor
const int LED_MOTION = 7;       // Output Pin for motion LED
const int LED_SOUND = 8;        // Output Pin for sound LED

// Variables to track the state of motion and sound detection
volatile int pirState = LOW;    // Initialize motion state as low 
volatile int soundState = LOW;  // Initialize sound state as low

// Variables to store the current readings from motion and sound sensors
int motionVal = 0;              // Variable for reading the pin status of motion sensor
int soundVal = 0;               // Variable for reading the pin status of sound sensor

void setup() {
  // Set pin modes for motion sensor and sound sensor with internal pull-up resistors
  pinMode(inputPinMotion, INPUT_PULLUP);
  pinMode(inputPinSound, INPUT_PULLUP);

  // Set pin modes for LED outputs
  pinMode(LED_MOTION, OUTPUT);
  pinMode(LED_SOUND, OUTPUT);

  // Enable PCINT interrupts for the pins used by motion and sound sensors
  PCICR |= (1 << PCIE2); // Enable PCINT2 group (corresponds to pins 0 to 7)
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19); // Enable PCINT18 (pin 2) and PCINT19 (pin 3) interrupts

  // Start serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Initialize the DHT sensor
  dht.begin();

  // Set up Timer1 for periodic interrupts
  cli();          // Disable global interrupts
  TCCR1A = 0;     // Set Timer1 control registers to initial values
  TCCR1B = 0;
  TCNT1 = 0;      // Initialize the counter value to 0
  OCR1A = 31250;  // Set the compare match value for 2hz timer
  TCCR1B |= (1 << WGM12);   // Set Timer1 to CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << CS12) | (1 << CS10);    // Set prescaler to 1024 and start the timer
  TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 compare match interrupt
  sei();          // Enable global interrupts
}

void loop() {

}

ISR(TIMER1_COMPA_vect) {
  // This interrupt service routine handles the timer interrupt
  // It will be triggered every 2 seconds to read the DHT22 sensor

  // Perform DHT readings and print the results
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

}

  ISR(PCINT2_vect) {
  // This interrupt service routine handles both motion and sound sensor changes
  // Determine which pin triggered the interrupt and handle accordingly

  // Sound sensor handling
  soundVal = digitalRead(inputPinSound);
  if (soundVal == HIGH) {
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

  // Motion sensor handling
  motionVal = digitalRead(inputPinMotion);
  if (motionVal == HIGH) {
    digitalWrite(LED_MOTION, HIGH);      // Turn motion LED ON
    if (pirState == LOW) {
      // Print Motion Detected only when the state changes from LOW to HIGH (not continuously)
      Serial.println("Motion detected!");
      pirState = HIGH;
    }
  } else {
    digitalWrite(LED_MOTION, LOW);       // Turn motion LED OFF
    if (pirState == HIGH){
      // Print Motion Ended only when the state changes from HIGH to LOW (not continuously)
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}
