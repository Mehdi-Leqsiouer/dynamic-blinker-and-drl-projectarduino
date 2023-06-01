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

//state of turn signal and DRL to avoid overriding state (low or high)
bool state_drl = true;
bool state_turnsignal = false;
bool state_drl_low = false;

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

  // if turn signal is ON,turn off DRL and turn on turn signal
  if (buttonTurnSignal == HIGH)
  {
    if (state_drl || state_drl_low) {
      digitalWrite(pwmPin, LOW);
      state_drl = false;
      state_drl_low = false;
    }
    
    TurnOnTurnSignal();
    state_turnsignal = true;
  }
  else { // Turn off the LEDs if buttonTurnSignal is not pressed and turn on back DRL
    if (state_turnsignal) {
      TurnOffTurnSignal();
      state_turnsignal = false;
    }
    if (!state_drl || !state_drl_low)  {
      TurnOnDRL();
    }
  }
}

//function to turn on DRL
//if position light or low_beam is active then we dim the brightness
//else we turn on DRL full brightness
void TurnOnDRL() {
  buttonLowBeam = digitalRead(pinLowBeam);
  buttonPosition = digitalRead(pinPosition);
  if ((buttonLowBeam == HIGH || buttonPosition == HIGH) && (!state_drl_low)) {
    analogWrite(pwmPin, brightness_lower);
    state_drl_low = true;
    state_drl = false;
  }
  else if (buttonLowBeam == LOW && buttonPosition == LOW) {
    if (!state_drl) {
      analogWrite(pwmPin, full_brightness);
      state_drl = true;
      state_drl_low = false;
    }
  }
  
}

//function to turn on yellow leds sequential. The last led is different because not connected to the shift register
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

//function to turn off the yellow leds
void TurnOffTurnSignal() {
  leds = 0;
  digitalWrite(last_led, LOW);
  updateShiftRegister();
}

//function for starting animation when arduino is powered
void startAnimation() {
  TurnOnTurnSignal();
  TurnOffTurnSignal();
  int i = 255;
  while ( i > 0 ){
    analogWrite( pwmPin, i );
    delay( WAITTIME );
    i = i - STEP;
  }
  i = 0;
  while ( i <= 255 ){
    analogWrite( pwmPin, i );
    delay( WAITTIME );
    i = i + STEP;
  }
}
