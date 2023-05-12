#include <ShiftRegister.h> // Library for shift register control (e.g., 74HC595)

// Constants for shift register pins
const int SER_PIN = 2;    // Serial data input
const int SRCLK_PIN = 3;  // Shift register clock input
const int RCLK_PIN = 4;   // Storage register clock input

// Constants for constant current driver and white LEDs
const int WHITE_LED_PIN = 5;     // PWM output pin for white LEDs
const int WHITE_LED_MAX_BRIGHTNESS = 255; // Maximum brightness value for white LEDs

// Constants for constant current driver and yellow LEDs
const int YELLOW_LED_PIN = 6;    // Output pin for yellow LEDs
const int NUM_YELLOW_LEDS = 10;  // Number of yellow LEDs connected
int delayTurnLedAnim = 1; //durée de chaque led dans la sequence cligno
int delayTurnLedOff = 350;

// Input pins for different signals
const int DRL_PIN = 7;           // Input pin for DRL signal
const int TURN_SIGNAL_PIN = 8;   // Input pin for turn signal
const int LOW_BEAM_PIN = 9;      // Input pin for low beam signal

// Variables to store the state of input signals
bool drlActive = false;
bool turnSignalActive = false;
bool lowBeamActive = false;

// Object for controlling the shift register
ShiftRegister sr(SER_PIN, SRCLK_PIN, RCLK_PIN);

void setup() {
  // Initialize the input and output pins
  pinMode(DRL_PIN, INPUT);
  pinMode(TURN_SIGNAL_PIN, INPUT);
  pinMode(LOW_BEAM_PIN, INPUT);
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  // Set the initial state of the white LEDs and yellow LEDs
  analogWrite(WHITE_LED_PIN, 0);  // Turn off the white LEDs
  sr.clear();                     // Turn off all the yellow LEDs initially
  sr.update();
}

void loop() {
  // Read the state of the input signals
  bool drlActive = digitalRead(DRL_PIN) == HIGH;
  bool turnSignalActive = digitalRead(TURN_SIGNAL_PIN) == HIGH;
  bool lowBeamActive = digitalRead(LOW_BEAM_PIN) == HIGH;

  // DRL and Turn Signal active: Activate yellow LEDs in dynamic sequence
  if (drlActive && turnSignalActive) {
    analogWrite(WHITE_LED_PIN, 0);  // Turn off the white LEDs
    activateYellowLEDsDynamic();
  }
  // DRL and Low Beam active: Reduce brightness of white LEDs
  else if (drlActive && lowBeamActive) {
    reduceBrightnessWhiteLEDs();
  }
  // Only DRL active: Turn on white LEDs at full brightness
  else if (drlActive) {
    activateWhiteLEDsFullBrightness();
  }
  // Only Turn Signal active: Turn off white LEDs
  else if (turnSignalActive) {
    analogWrite(WHITE_LED_PIN, 0);  // Turn off the white LEDs
  }
  // No conditions met: Turn off all LEDs
  else {
    analogWrite(WHITE_LED_PIN, 0);  // Turn off the white LEDs
  }
}



// Function to activate yellow LEDs in dynamic sequence
void activateYellowLEDsDynamic() {
  for (int i = 0; i < NUM_YELLOW_LEDS; i++) {
    sr.set(i, HIGH);  // Turn on individual yellow LEDs one by one
    sr.update();
    delay(delayTurnLedAnim);  // Adjust the delay time for the desired sequence speed
  }
  delay(delayTurnLedOff)
  sr.clear();  // Turn off all the yellow LEDs
  sr.update();
}

// Function to reduce the brightness of white LEDs
void reduceBrightnessWhiteLEDs() {
  analogWrite(WHITE_LED_PIN, WHITE_LED_MAX_BRIGHTNESS / 2);  // Adjust the brightness level as needed
}

// Function to activate white LEDs at full brightness
void activateWhiteLEDsFullBrightness() {
  analogWrite(WHITE_LED_PIN, WHITE_LED_MAX_BRIGHTNESS);
}
