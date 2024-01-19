// TODO:
// fix retrigger delay
// Figure out why dynamic fade is not working

#include "debug.h"
#include "pins.h"
#include "constants.h"

typedef struct {
    int contactPin;
    int laserPin;

    bool previousState;
    bool debounceState = false;
    int debounceTime = 0;
    float laserState = 0;
} State;

State state0;
State state1;

// ##################################
// Setup
// ##################################
void setup() {
    #ifdef DEBUG
    pinMode(LED_DEBUG, OUTPUT);
    #endif

    setupState(&state0, CONTACT_PIN0, LASER_PIN0);
    setupState(&state1, CONTACT_PIN1, LASER_PIN1);
}

// Setup the state for a laser
void setupState(State *state, int contactPin, int laserPin) {
    state->contactPin = contactPin;
    state->laserPin = laserPin;

    pinMode(contactPin, INPUT);
    pinMode(laserPin, OUTPUT);
}

// ##################################
// Main loop
// ##################################
void loop() {
    processContact(&state0);
    processContact(&state1);

    processFade(&state0);
    processFade(&state1);

    delay(PROGRAM_DELAY);
}

// Processes the contact change
void processContact(State *state) {
    // Exit if the laser is already on
    if (state->laserState > 0) {
        return;
    }

    int currentState = checkContact(state->contactPin);

    // Debounce
    if (currentState == state->debounceState) {
        state->debounceTime += PROGRAM_DELAY;
    } else {
        state->debounceState = currentState;
        state->debounceTime = 0;
    }

    if (state->debounceTime >= CONTACT_DEBOUNCE && currentState != state->previousState) {
        enableLaser(state);
    }
}

// Checks if a contact is connected or not
// NOTE: these will be LOW when connected and HIGH when disconnected
bool checkContact(int pin) {
    if (digitalRead(pin) == LOW) {
        return true;
    } else {
        return false;
    }
}

// Enable the laser
void enableLaser(State *state) {
    state->laserState = LASER_INIT_BRIGHTNESS;
    state->debounceTime = 0;
    analogWrite(state->laserPin, state->laserState);
}

void processFade(State *state) {
    // Don't fade if the laser is off
    if (state->laserState == 0) {
        return;
    }

    //state->laserState -= LASER_INIT_BRIGHTNESS / (LASER_FADE_TIME / PROGRAM_DELAY);
    state->laserState -= 0.5;

    if (state->laserState <= 0) {
        state->laserState = 0;
        state->previousState = checkContact(state->contactPin);
        return;
    }

    analogWrite(state->laserPin, (int) state->laserState);
}
