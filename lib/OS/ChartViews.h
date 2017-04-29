#ifndef ChartViews_h
#define ChartViews_h

#define MAX_VALUES 64

#include <OSView.h>

class LineChart : public OSView {
public:

  LineChart(int   x,
            int   y,
            int   width,
            int   height,
            float minRange);
  void addValue(float value);
  void draw(U8G2 *u8g2);

private:

  int _x, _y, _width, _height, _numEntries;
  float _values[MAX_VALUES] = { 0 };
  float _minRange;
  int _valueIndex = 0;
};

#endif // ifndef ChartViews_h
