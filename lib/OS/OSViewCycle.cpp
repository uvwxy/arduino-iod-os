#include <Arduino.h>
#include <OSViewCycle.h>
#include <OSButtons.h>

OSViewCycle::OSViewCycle(int pin) : OSView() {
  _cyclePin = pin;
}

void OSViewCycle::setOverlay(OSView *overlay) {
  _overlay = overlay;
}

bool OSViewCycle::click(int pin) {
  bool consumed = false;

  // first check if current screen wants to handle click
  if ((_vIndex >= 0) &&
      (_vIndex < MAX_CHILD_VIEWS) &&
      (_childViews[_vIndex] != NULL)) {
    consumed = _childViews[_vIndex]->click(pin);
  }

  // then check if the cycle can consume the click
  if (!consumed && (pin == _cyclePin)) {
    _vIndex++;
    _vIndex  = (_vIndex % _childCount);
    consumed = true;
  }

  return consumed;
}

void OSViewCycle::draw(U8G2 *u8g2) {
  // only draw active child view
  if ((_vIndex >= 0) &&
      (_vIndex < MAX_CHILD_VIEWS) &&
      (_childViews[_vIndex] != NULL)) {
    _childViews[_vIndex]->draw(u8g2);
  }

  if (_overlay != NULL) {
    _overlay->draw(u8g2);
  }
}
