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

void OSTime::tick() {
  unsigned long millisNow = millis();

  if (millisLast < millisNow) {
    int d = (millisNow - millisLast) / 1000;
    uptimeSeconds += d;
    unixtime      += d;
  } else {
    // special case
    int d = (/*overflow*/ millisNow + /*rest*/ (ULONG_MAX - millisLast)) / 1000;
    uptimeSeconds += d;
    unixtime      += d;
  }
  millisLast = millisNow;
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
