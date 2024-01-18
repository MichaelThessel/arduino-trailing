const unsigned int buttonPin = 4;
const unsigned int buttonPin2 = 3;

const unsigned int ledPin = 0;
const unsigned int ledPin2 = 1;

bool buttonState = false;
bool checkState = true;
bool buttonState2 = false;
bool checkState2 = true;

unsigned int timer = 0;
unsigned int timer2 = 0;
const unsigned int duration = 200; // delay time after light faded out
const unsigned int delayTime = 2; // program run rate

int fadeValue = 0;
int fadeValue2 = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);

  
  // ---------------- light 1 ---------------- //
  
  if (buttonState == true && checkState == true) {
    fadeValue = 255; // turn light on
    checkState = false;
  }

  if (fadeValue > 0 && checkState == false) {
    fadeValue --; // fade out light
  }

  else if (fadeValue < 2) {
    fadeValue = 0; // keep light off if switch is stuck
  }

  analogWrite(ledPin, fadeValue);

  


  // ---------------- light 2 ---------------- //

  if (buttonState2 == true && checkState2 == true) {
    fadeValue2 = 255; // turn light on
    checkState2 = false;
  }

  if (fadeValue2 > 0 && checkState2 == false) {
    fadeValue2 --; // fade out light
  }

  else if (fadeValue2 < 2) {
    fadeValue2 = 0; // keep light off if switch is stuck
  }

  analogWrite(ledPin2, fadeValue2);

  // ---------------- check button 1 ---------------- //
  
  if (buttonState == false && checkState == false && fadeValue == 0) {
    if (timer < duration) {
      timer ++;
    }
    else {
      checkState = true; 
      timer = 0; 
    }
  }


  // ---------------- check button 2 ---------------- //

  if (buttonState2 == false && checkState2 == false && fadeValue2 == 0) {
    if (timer2 < duration) {
      timer2 ++;
    }
    else {
      checkState2 = true; 
      timer2 = 0; 
    }
  }

  delay(delayTime);

}
