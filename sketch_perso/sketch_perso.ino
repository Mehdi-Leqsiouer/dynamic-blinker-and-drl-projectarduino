//www.elegoo.com
//2016.12.8


// pin locations
int pwmPin = 11;
int pinLowBeam = 4;
int pinPosition = 3;
int pinTurnSignal = 9;
//int pinDRL = 7;
int latchPin = 12;      // (12) ST_CP [RCK] on 74HC595
int clockPin = 8;      // (8) SH_CP [SCK] on 74HC595
int dataPin = 13;     // (13) DS [S1] on 74HC595
int last_led = 2;


//variable for input pin state
int buttonLowBeam = 0;
int buttonPosition = 0;
int buttonTurnSignal = 0;
//int buttonDRL = 0;

//delay of blinker to adjust in the cars
int delayStart = 100;
int tDelay = 70;
int delayEndSignal = 350;

//number of leds for sequential blinker
int numLeds = 6;

//brightness values
int full_brightness = 255;
int brightness_lower = 64;

//variable for the starting animation of white leds
const int WAITTIME = 20;
const int STEP = 2;

//byte leds for shifter register
byte leds = 0;

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

void setup()
{
//Serial.begin(9600);
pinMode(pwmPin, OUTPUT);
pinMode(pinLowBeam, INPUT);
pinMode(pinTurnSignal, INPUT);
//pinMode(pinDRL, INPUT);
pinMode(latchPin, OUTPUT);
pinMode(dataPin, OUTPUT);  
pinMode(clockPin, OUTPUT);
pinMode(last_led, OUTPUT);
startAnimation();
}


// main loop
void loop() {
  buttonTurnSignal = digitalRead(pinTurnSignal);

  if (buttonTurnSignal == HIGH)
  {
    digitalWrite(pwmPin, LOW);
    TurnOnTurnSignal();
  }
  else { // Turn off the LEDs if buttonTurnSignal is not pressed
    TurnOffTurnSignal();
    TurnOnDRL();
  }
}

void TurnOnDRL() {
  buttonLowBeam = digitalRead(pinLowBeam);
  buttonPosition = digitalRead(pinPosition);
  if (buttonLowBeam == HIGH || buttonPosition == HIGH) {
    analogWrite(pwmPin, brightness_lower);
  }
  else {
    analogWrite(pwmPin, full_brightness);
  }
  
}

void TurnOnTurnSignal() {
  TurnOffTurnSignal();
  delay(tDelay);
  for (int i = 0; i < numLeds; i++)
  {
    bitSet(leds, i);
    updateShiftRegister();
    if (i == numLeds-1) {
      delay(0);
    }
    else {
      delay(tDelay);
    }
  }
  digitalWrite(last_led, HIGH);
  delay(tDelay);
  delay(delayEndSignal);
}

void TurnOffTurnSignal() {
  leds = 0;
  digitalWrite(last_led, LOW);
  updateShiftRegister();
}

void startAnimation() {
  TurnOnTurnSignal();
  TurnOffTurnSignal();
  int i = 0;
  while ( i <= 255 ){
    analogWrite( pwmPin, i );
    delay( WAITTIME );
    i = i + STEP;
  }
}
