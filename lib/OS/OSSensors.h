#ifndef OSSensors_h
#define OSSensors_h

#include <Arduino.h>

class OSSensors {
public:

  OSSensors(bool  metric,
            float pressureUnit);

  void   setup();
  void   readValues();
  float  getPres();
  float  getTemp();
  float  getHum();
  float  getHeight();

  char** getTempUnit();
  char** getHumUnit();
  char** getPresUnit();
  char** getAltUnit();

private:

  bool _started = false;
  bool _metric  = true;

  float _pres = NAN;
  float _temp = NAN;
  float _hum  = NAN;
  float _alt  = NAN;

  // unit: B000 = Pa,  B001 = hPa,  B010 = Hg,    B011 = atm,
  //       B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
  float _pressureUnit = 1;

  char *tempUnit;
  char *humUnit;
  char *presUnit;
  char *altUnit;
};


#endif // ifndef OSSensors_h
