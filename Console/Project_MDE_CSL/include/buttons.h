#ifndef BUTTONS_H
#define BUTTONS_H 

#include "Adafruit_Keypad.h"

const byte ROWS = 4; // rows
const byte COLS = 4; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 10, 11, 12}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad menukeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void initkeypad(){
    menukeypad.begin();
}

#define TAFLS_BUTTON (A0)
#define TATOS_BUTTON (A1)
#define TAPTS1_BUTTON (A2)
#define TAPTS2_BUTTON (A3)
#define TAPTS3_BUTTON (A4)
#define HORN_BUTTON (A5)
#define GAMECLK_BUTTON (A6)
#define CORRECT_BUTTON (A7)
#define TBFLS_BUTTON (A8)
#define TBTOS_BUTTON (A9)
#define TBPTS1_BUTTON (A10)
#define TBPTS2_BUTTON (A11)
#define TBPTS3_BUTTON (A12)

void initbuttons(){
    pinMode(TAFLS_BUTTON, INPUT);
    pinMode(TATOS_BUTTON, INPUT);
    pinMode(TAPTS1_BUTTON, INPUT);
    pinMode(TAPTS2_BUTTON, INPUT);
    pinMode(TAPTS3_BUTTON, INPUT);
    pinMode(HORN_BUTTON, INPUT);
    pinMode(GAMECLK_BUTTON, INPUT);
    pinMode(CORRECT_BUTTON, INPUT);
    pinMode(TBFLS_BUTTON, INPUT);
    pinMode(TBTOS_BUTTON, INPUT);
    pinMode(TBPTS1_BUTTON, INPUT);
    pinMode(TBPTS2_BUTTON, INPUT);
    pinMode(TBPTS3_BUTTON, INPUT);
}

bool isButtonPressedOnce(int buttonPin) {
    static bool buttonState = false; // Tracks the current state of the button
    static bool lastButtonState = false; // Tracks the last state of the button
    static unsigned long lastDebounceTime = 0; // Tracks the last debounce time
    const unsigned long debounceDelay = 50; // Debounce time in milliseconds

    // Read the current state of the button
    bool currentButtonState = digitalRead(buttonPin);

    // Check if the button state has changed
    if (currentButtonState != lastButtonState) {
        lastDebounceTime = millis(); // Reset the debounce timer
    }

    // If enough time has passed and the state is stable
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Only register the press if the button is pressed and was not pressed before
        if (currentButtonState && !buttonState) {
            buttonState = true; // Update the button state
            lastButtonState = currentButtonState;
            return true; // Return true to indicate a single press was detected
        } else if (!currentButtonState) {
            buttonState = false; // Reset the button state when released
        }
    }

    lastButtonState = currentButtonState; // Update the last state
    return false; // No valid press detected
}

/*bool isButtonPressedOnce2(int buttonPin) {
    static bool buttonState = false; // Tracks the current state of the button
    static bool lastButtonState = false; // Tracks the last state of the button
    static unsigned long lastDebounceTime = 0; // Tracks the last debounce time
    const unsigned long debounceDelay = 50; // Debounce time in milliseconds

    // Read the current state of the button
    bool currentButtonState = digitalRead(buttonPin);

    // Check if the button state has changed
    if (currentButtonState != lastButtonState) {
        lastDebounceTime = millis(); // Reset the debounce timer
    }

    // If enough time has passed and the state is stable
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Only register the press if the button is pressed and was not pressed before
        if (currentButtonState && !buttonState) {
            buttonState = true; // Update the button state
            lastButtonState = currentButtonState;
            return true; // Return true to indicate a single press was detected
        } else if (!currentButtonState) {
            buttonState = false; // Reset the button state when released
        }
    }

    lastButtonState = currentButtonState; // Update the last state
    return false; // No valid press detected
}*/

bool buttondebounce(int buttonPin) {
  int buttonState;            // the current reading from the input pin
  int lastButtonState = LOW;  // the previous reading from the input pin

  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 50;
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  return lastButtonState;
}

#endif // BUTTONS_H