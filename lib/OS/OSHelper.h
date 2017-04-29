#ifndef OSHelper_h
#define OSHelper_h

#include <Arduino.h>
#include <Limits.h>

static char* str2char(String s) {
  char *result = new char[s.length()];

  strcpy(result, s.c_str());
  return result;
}

static void float2char(float f, int d, char *& str) {
  strcpy(str, String(f, d).c_str());
}

static float getMin(float values[], int valuesLength, int start, int count) {
  float m = values[0];

  for (int i = 0; i < count; i++) {
    int index = start - i;
    index = index < 0 ? index + valuesLength : index;
    index = index % valuesLength;
    m     = m < values[index] ? m : values[index];
  }

  return m;
}

static float getMax(float values[], int valuesLength, int start, int count) {
  float m = values[start];

  for (int i = 0; i < count; i++) {
    int index = start - i;
    index = index < 0 ? index + valuesLength : index;
    index = index % valuesLength;
    m     = m > values[index] ? m : values[index];
  }

  return m;
}

#endif // ifndef OSHelper_h
