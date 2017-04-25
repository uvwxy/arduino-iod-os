#ifndef OSTimers_h
#define OSTimers_h

#include <Arduino.h>

#define MAX_TIMERS 16


class OSTimer {
public:

  OSTimer(void(*callback)(void),
          unsigned long millisInterval);
  bool checkIntervall(unsigned long millisSinceLastCall);

private:

  unsigned long _millisInterval = 0;
  unsigned long _millisPassed = 0;
  void (*_callback)();
};

class OSTimers {
public:

  int  registerTimer(OSTimer *timer);
  void checkTimers(unsigned long millisSinceLastCall);

private:

  OSTimer *_timers[MAX_TIMERS] = {};
};


#endif // ifndef OSTimers_h
