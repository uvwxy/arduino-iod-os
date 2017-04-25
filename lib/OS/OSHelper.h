#ifndef OSHelper_h
#define OSHelper_h

#include <Arduino.h>

static char* str2char(String s) {
  char *result = new char[s.length()];

  strcpy(result, s.c_str());
  return result;
}

static void float2char(float f, int d, char *& str) {
  strcpy(str, String(f, d).c_str());
}

#endif // ifndef OSHelper_h
