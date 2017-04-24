#include <Arduino.h>
#include <OSButtons.h>

void OSButtons::setup() {
  _button_map[0] = D3;
  _button_map[1] = D4;

  pinMode(D3, INPUT);
  digitalWrite(D3, HIGH); // turn on pullup resistors
  pinMode(D4, INPUT);
  digitalWrite(D4, HIGH); // turn on pullup resistors
}

void handleClick(int pin, bool(*callbacks[])(int)) {
  bool consumed = false;

  if (digitalRead(pin) == LOW) {
    for (int i = 0; i < MAX_BUTTON_CALLBACKS; i++) {
      if ((callbacks[i] != NULL) && !consumed) {
        consumed = callbacks[i](pin);
      }
    }
  }
}

void OSButtons::handleButtonClicks() {
  handleClick(D3, _buttonCallbacks[0]);
  handleClick(D4, _buttonCallbacks[1]);
}

int OSButtons::registerButtonClick(int pin, bool (*callback)(int)) {
  int index = -1;

  for (int i = 0; i < MAX_BUTTONS; i++) {
    if (_button_map[i] == pin) {
      index = i;
    }
  }

  if (index == -1) {
    return -1; // pin not found in map
  }

  for (int i = 0; i < MAX_BUTTON_CALLBACKS; i++) {
    if (_buttonCallbacks[index][i] == NULL) {
      _buttonCallbacks[index][i] = callback;
      return i;
    }
  }

  return -2; // no callbacks found
}
