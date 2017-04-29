#include <ChartViews.h>
#include <OSHelper.h>

#include <Math.h>

LineChart::LineChart(int   x,
                     int   y,
                     int   width,
                     int   height,
                     float minRange) {
  _x        = x;
  _y        = y;
  _width    = width;
  _height   = height;
  _minRange = minRange;
}

void LineChart::addValue(float value) {
  // move index to next position and overwrite value
  _values[_valueIndex] = value;
  _valueIndex++;
  _valueIndex = _valueIndex % MAX_VALUES;
  _numEntries = _numEntries <= MAX_VALUES ? _numEntries + 1 : _numEntries;
}

void LineChart::draw(U8G2 *u8g2) {
  // get first index and follower
  int firstIndex = (_valueIndex - 1);

  firstIndex = firstIndex < 0 ? firstIndex + MAX_VALUES : firstIndex;
  int nextIndex = (firstIndex - 1);

  nextIndex = nextIndex < 0 ? nextIndex + MAX_VALUES : nextIndex;

  float __max = getMax(_values, MAX_VALUES, firstIndex, _numEntries);
  float __min = getMin(_values, MAX_VALUES, firstIndex, _numEntries);

  float shift = -__min;
  float range = __max + shift;

  // the 0.00000001 is to trick hlines into the middle
  range = range == 0.0 ? 0.00000001 : range;
  float drawRange = range < _minRange ? _minRange : range;

  for (int i = 1; i < MAX_VALUES; i++) {
    if (i >= _numEntries) {
      // don't draw zeros on startup
      break;
    }

    // normalization, drawing the line in the middle of the screen
    float rm = range / drawRange;
    float ro = _height * (1.0 - (range / drawRange)) / 2.0;

    float x1 = _width - ((float)i - 1.0) / ((float)MAX_VALUES - 1.0) * _width;
    float y1 = _height - _height * ((_values[firstIndex] + shift) / range) * rm -
               ro;
    float x2 = _width - ((float)i) / ((float)MAX_VALUES - 1.0) * _width;
    float y2 = _height - _height * ((_values[nextIndex] + shift) / range) * rm -
               ro;
    u8g2->drawLine(_x + x1, _y + y1, _x + x2, _y + y2);

    // get next pair
    firstIndex = nextIndex;
    nextIndex  = (nextIndex - 1);
    nextIndex  = nextIndex < 0 ? nextIndex + MAX_VALUES : nextIndex;
  }

  u8g2->drawFrame(_x, _y, _width, _height);

  OSView::draw(u8g2);
}
