#include <Arduino.h>
#include <OSButtons.h>

void OSButtons::setup(int clickTimeout) {
  _clickTimeout = clickTimeout;

  _buttonMap[0] = D3;
  _buttonMap[1] = D4;

  for (int i = 0; i < MAX_BUTTONS; i++) {
    _buttonLastState[i] = 0;
  }

  // TODO: move to root setup(), button mapping might change in the future
}

void OSButtons::handleClick(int pin, int index) {
  bool consumed = false;

  // button is active
  if (digitalRead(pin) == LOW) {
    // click is enabled (no timeout)
    if (_buttonLastState[index] == 0) {
      // set timeout
      _buttonLastState[index] = _clickTimeout;

      // send click event
      for (int i = 0; i < MAX_BUTTON_CALLBACKS; i++) {
        if ((_buttonCallbacks[index][i] != NULL) && !consumed) {
          consumed = _buttonCallbacks[index][i](pin);
        }
      }
    } else if (_buttonLastState[index] > 0) {
      // reduce timeout
      _buttonLastState[index]--;
    }
  }
}

void OSButtons::handleButtonClicks() {
  // TODO: similar as the todo in the setup, this could be more generic
  handleClick(D3, 0);
  handleClick(D4, 1);
}

int OSButtons::registerButtonClick(int pin, bool (*callback)(int)) {
  int index = -1;

  for (int i = 0; i < MAX_BUTTONS; i++) {
    if (_buttonMap[i] == pin) {
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
