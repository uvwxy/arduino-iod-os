#include <Arduino.h>
#include <BME280I2C.h>

#include <OSHelper.h>
#include <OSSensors.h>

#include <Wire.h>

/*
 * Sensors implementation using BME280P on I2C
 */

BME280I2C bme;

char const *u_percent = "%";

char const *u_celsius    = "C";
char const *u_fahrenheit = "F";

char const *u_meter = "m";
char const *u_foot  = "ft";

char const *u_millibar = "mb";

OSSensors::OSSensors(bool metric, float pressureUnit) {
  _metric       = metric;
  _pressureUnit = pressureUnit;

  humUnit = str2char(u_percent);

  if (metric) {
    tempUnit = str2char(u_celsius);
    altUnit  =  str2char(u_meter);
  }

  if (pressureUnit == 1) {
    presUnit = str2char(u_millibar);
  }
}

char ** OSSensors::getAltUnit() {
  return &altUnit;
}

char ** OSSensors::getPresUnit() {
  return &presUnit;
}

char ** OSSensors::getTempUnit() {
  return &tempUnit;
}

char ** OSSensors::getHumUnit() {
  return &humUnit;
}

void OSSensors::setup() {
  _started = bme.begin();
}

void OSSensors::readValues() {
  if (_started) {
    bme.read(_pres, _temp, _hum, _metric, _pressureUnit);
    _alt = bme.alt(_metric);
  }
}

float OSSensors::getPres()   {
  return _pres;
}

float OSSensors::getTemp()   {
  return _temp;
}

float OSSensors::getHum()    {
  return _hum;
}

float OSSensors::getHeight() {
  return _alt;
}
