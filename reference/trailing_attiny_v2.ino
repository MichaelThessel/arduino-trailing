// constant variables
const int buttonMin = 20;      // minimum switch contact time to start fade cycle
const int debounce = 200;      // minimum switch off time before able to retrigger
const int delayTime = 10;      // program delay rate
const int fadeRate = 140;      // rate for exponential fade time

// pins
const int buttonPin0 = 4;
const int buttonPin1 = 3;
const int ledPin0 = 0;
const int ledPin1 = 1;

// button variables
bool buttonRead0;
bool buttonRead1;
bool buttonCheck0 = false;
bool buttonCheck1 = false;
uint16_t buttonON0 = 0;
uint16_t buttonON1 = 0;
uint16_t buttonOFF0 = 0;
uint16_t buttonOFF1 = 0;

// led variables
bool ledCheck0 = false;
bool ledCheck1 = false;
float fadeValue0 = 0;
float fadeValue1 = 0;

void setup() {
  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
}

void loop() {
  buttonRead0 = digitalRead(buttonPin0);
  buttonRead1 = digitalRead(buttonPin1);

  // ---------------- light 1 ---------------- //

  // start button ON counter
  if (buttonRead0 == LOW && buttonON0 <= buttonMin) {
    buttonON0 ++;
    buttonOFF0 = 0;
  }
  // start button OFF counter
  else if (buttonRead0 == HIGH && buttonOFF0 <= debounce) {
    buttonON0 = 0;
    buttonOFF0 ++;
  }

  // compare button ON counter to minimum button time
  if (buttonON0 >= buttonMin && ledCheck0 == false && buttonCheck0 == false) {
    fadeValue0 = 255; // set led on
    ledCheck0 = true;
    buttonCheck0 = true;
  }

  // compare button OFF counter to debounce time
  if (buttonOFF0 >= debounce) {
    buttonCheck0 = false;
  }

  // fade led off
  if (fadeValue0 > 0) {
    fadeValue0 -= 0.2;                    // linear fade
    //fadeValue0 -= (fadeValue0 / fadeRate);  // exponential fade
  }
  else {
    ledCheck0 = false;
  }

  if (fadeValue0 < 1) {
    fadeValue0 = 0;
  }

  // ---------------- light 2 ---------------- //

  // start button ON counter
  if (buttonRead1 == LOW && buttonON1 <= buttonMin) {
    buttonON1 ++;
    buttonOFF1 = 0;
  }
  // start button OFF counter
  else if (buttonRead1 == HIGH && buttonOFF1 <= debounce) {
    buttonON1 = 0;
    buttonOFF1 ++;
  }

  // compare button ON counter to minimum button time
  if (buttonON1 >= buttonMin && ledCheck1 == false && buttonCheck1 == false) {
    fadeValue1 = 255; // set led on
    ledCheck1 = true;
    buttonCheck1 = true;
  }

  // compare button OFF counter to debounce time
  if (buttonOFF1 >= debounce) {
    buttonCheck1 = false;
  }

  // fade led off
  if (fadeValue1 > 0) {
    fadeValue1 -= 0.2;                    // linear fade
    //fadeValue1 -= (fadeValue1 / fadeRate);  // exponential fade
  }
  else {
    ledCheck1 = false;
  }

  if (fadeValue1 < 1) {
    fadeValue1 = 0;
  }

  // ----------------------------------------- //

  analogWrite(ledPin0, fadeValue0);
  analogWrite(ledPin1, fadeValue1);

  delay(delayTime);
}
