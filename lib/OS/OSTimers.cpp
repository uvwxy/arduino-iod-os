#include <OSTimers.h>
#include <Arduino.h>

OSTimer::OSTimer(void(*callback)(void),
                 unsigned long millisInterval) {
  _callback       = callback;
  _millisInterval = millisInterval;
}

bool OSTimer::checkIntervall(unsigned long millisSinceLastCall) {
  _millisPassed += millisSinceLastCall;

  if (((_millisPassed / _millisInterval) > 0) && (_callback != NULL)) {
    _callback();
    _millisPassed = _millisPassed % _millisInterval;
    return true;
  }

  return false;
}

int OSTimers::registerTimer(OSTimer *timer)         {
  for (int i = 0; i < MAX_TIMERS; i++) {
    if (_timers[i] == NULL) {
      _timers[i] = timer;
      return i;
    }
  }

  return -1;
}

void OSTimers::checkTimers(unsigned long millisSinceLastCall) {
  for (int i = 0; i < MAX_TIMERS; i++) {
    if (_timers[i] != NULL) {
      _timers[i]->checkIntervall(millisSinceLastCall);
    }
  }
}
