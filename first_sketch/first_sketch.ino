//www.elegoo.com
//2016.12.8

// Define Pins
#define BLUE 11
#define GREEN 12
#define RED 13

#define BLUE2 5
#define GREEN2 6
#define RED2 7
int buttonApin = 2;
bool blinker_active = false;
int buttonAstate = HIGH;

// define the previous state of the button (HIGH or LOW)
int buttonAprevious = HIGH;


void setup()
{
pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
pinMode(RED2, OUTPUT);
pinMode(GREEN2, OUTPUT);
pinMode(BLUE2, OUTPUT);
pinMode(buttonApin, INPUT_PULLUP);
analogWrite(RED, 255);
analogWrite(GREEN, 255);
analogWrite(BLUE, 255);
analogWrite(RED2, 255);
analogWrite(GREEN2, 255);
analogWrite(BLUE2, 255);
}

// define variables
int redValue;
int greenValue;
int blueValue;

// main loop
void loop() {
  buttonAstate = digitalRead(buttonApin);
  // blink the RGB LEDs with a "car blinker" effect
  // check for a button press (change from LOW to HIGH)
  if (buttonAstate == HIGH && buttonAprevious == LOW) {
    // toggle the blinker state
    blinker_active = !blinker_active;
  }

  // store the current button state as the previous state
  buttonAprevious = buttonAstate;

  if (blinker_active)
  {
    for (int i = 0; i < 3; i++) {
      // turn on LED1 with yellow color
      analogWrite(RED, 255);
      analogWrite(GREEN, 255);
      analogWrite(BLUE, 0);
      // turn on LED2
      delay(200);
      analogWrite(RED2, 255);
      analogWrite(GREEN2, 255);
      analogWrite(BLUE2, 0);
      delay(200); // wait for a short time
      // turn on LED2 with yellow color
      // turn off LED2
      analogWrite(RED2, 0);
      analogWrite(GREEN2, 0);
      analogWrite(BLUE2, 0);
      analogWrite(RED, 0);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      delay(200); // wait for a short time
    }
  }
  else 
  {
    analogWrite(RED2, 255);
    analogWrite(GREEN2, 255);
    analogWrite(BLUE2, 255);
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);

    analogWrite(RED, 51);    // 20% of 255
    analogWrite(GREEN, 51);  // 20% of 255
    analogWrite(BLUE, 51); 

  }
}

