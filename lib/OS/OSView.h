#ifndef OSView_h
#define OSView_h


#include "Arduino.h"
#include "U8g2lib.h"

#define MAX_OVERLAY_VIEWS 4
#define MAX_CHILD_VIEWS 8

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

class OSView {
public:

  OSView();
  OSView     * addView(OSView *view);
  bool         broadcastClick(int pin);
  virtual void draw(U8G2 *u8g2);
  virtual bool click(int pin) {
    return false;
  }

protected:

  OSView *_childViews[MAX_CHILD_VIEWS] = {};
  int _childCount = 0;
};

#endif // ifndef OSView_h
