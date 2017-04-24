#ifndef OSHelper_h
#define OSHelper_h

#include <Arduino.h>

static char* str2char(String s) {
  char *result = new char[s.length()];

  strcpy(result, s.c_str());
  return result;
}

#endif // ifndef OSHelper_h
