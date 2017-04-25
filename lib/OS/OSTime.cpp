#include <Arduino.h>
#include <OSTime.h>
#include <limits.h>
#include <OSHelper.h>

OSTime::OSTime() {
  millisLast    = 0;
  uptimeSeconds = 0;
  uptimeStr     = new char[12]; // xxxxhyymzzs0
  tick();
}

/**
 * Add all passed seconds (diffSeconds) to the counters. In most cases there are
 * milliseconds left (deltaMillis) we need to add next time.
 */
void OSTime::tick() {
  unsigned long millisNow = millis();
  unsigned long diffMillis = 0, diffSeconds = 0, deltaMillis = 0;

  if (millisLast < millisNow) {
    diffMillis  = millisNow - millisLast;
    diffSeconds = diffMillis / 1000;
    deltaMillis = diffMillis % 1000;

    uptimeSeconds += diffSeconds;
    unixtime      += diffSeconds;
  } else {
    // overflow case
    diffMillis  = /*overflow*/ millisNow + /*rest*/ (ULONG_MAX - millisLast);
    diffSeconds = diffMillis / 1000;
    deltaMillis = diffMillis % 1000;

    uptimeSeconds += diffSeconds;
    unixtime      += diffSeconds;
  }

  millisLast = millisNow - deltaMillis;
}

void OSTime::get24hStr(char *& str, int seconds) {
  // refresh string and return
  int h = seconds / 60 / 60;
  int m = (seconds / 60) % 60;
  int s = seconds % 60;

  String t =   (h < 10 ? "0" : "") + String(h) + ":" //
             + (m < 10 ? "0" : "") + String(m) + ":" //
             + (s < 10 ? "0" : "") + String(s);

  strcpy(str, t.c_str());
}

void OSTime::getUptimeStr(char *& str) {
  // make sure we dont display more than 8 chars width
  int u = uptimeSeconds % (99 * 60 * 60);

  get24hStr(str, u);
}

void OSTime::getUnixTimeStr(char *& str) {
  int u = (unixtime % (60 * 60 * 24)); // keep seconds of running day

  get24hStr(str, u);
}

unsigned long OSTime::getUnixTime() {
  return unixtime;
}

void OSTime::setUnixTime(unsigned long t) {
  unixtime = t;
}
