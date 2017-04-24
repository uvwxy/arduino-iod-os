#ifndef OSViewMatrix_h
#define OSViewMatrix_h

#include <Arduino.h>
#include <OSView.h>
#include <OSButtons.h>

#define MAX_COLUMNS 16

class OSViewCycle : public OSView {
public:

  OSViewCycle(int pin);
  bool click(int pin);
  void draw(U8G2 *u8g2);
  void setOverlay(OSView *overlay);

private:

  OSView *_overlay = NULL;
  bool _cycleCallback(int pin);
  int _cyclePin = -1;
  int _vIndex   = 0; // view index in array
};


#endif // ViewMatrix_h
