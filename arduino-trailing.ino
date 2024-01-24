#include "debug.h"
#include "pins.h"
#include "constants.h"

typedef struct {
    int contactPin;
    int laserPin;

    bool previousState;
    bool debounceState = false;
    unsigned int debounceTime = 0;
    float laserLevel = 0;
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

    // Turn on lasers initially to spot issues when powering on the system
    enableLaser(&state0);
    enableLaser(&state1);
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
    if (state->laserLevel > 0) {
        return;
    }

    int currentState = checkContact(state);

    // Debounce
    if (currentState == state->debounceState) {
        if (state->debounceTime < UINT_MAX) {
            state->debounceTime += PROGRAM_DELAY;
        }
    } else {
        state->debounceState = currentState;
        state->debounceTime = 0;
    }

    // If the currently detected change has persisted for the debounce period
    // and is different from the previously detected state turn on the laser
    if (state->debounceTime >= CONTACT_DEBOUNCE && currentState != state->previousState) {
        state->debounceTime = 0;
        state->previousState = currentState;

        if (!currentState) {
            enableLaser(state);
        }
    }
}

// Checks if a contact is connected or not
// NOTE: these will be LOW when connected and HIGH when disconnected
bool checkContact(State *state) {
    if (digitalRead(state->contactPin) == LOW) {
        return true;
    } else {
        return false;
    }
}

// Enable the laser
void enableLaser(State *state) {
    state->laserLevel = LASER_START_BRIGHTNESS;
    analogWrite(state->laserPin, state->laserLevel);
}

// Fade the laser over time
void processFade(State *state) {
    // Don't fade if the laser is off
    if (state->laserLevel == 0) {
        return;
    }

    state->laserLevel -= (float) LASER_START_BRIGHTNESS / (float) (LASER_FADE_TIME / (float) PROGRAM_DELAY);

    // Reset the laser
    if (state->laserLevel <= 0) {
        state->laserLevel = 0;
    }

    analogWrite(state->laserPin, (int) state->laserLevel);
}
