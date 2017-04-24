#include <OSView.h>
#include <U8g2lib.h>


OSView::OSView() {}

int OSView::addView(OSView *view) {
  for (int i = 0; i < MAX_CHILD_VIEWS; i++) {
    if (_childViews[i] == NULL) {
      _childViews[i] = view;
      _childCount++;
      return i;
    }
  }
  return -1;
}

bool OSView::broadcastClick(int pin) {
  bool consumed = false;

  for (int i = 0; i < MAX_CHILD_VIEWS; i++) {
    if (!consumed && (_childViews[i] != NULL)) {
      consumed = _childViews[i]->click(pin);
    }
  }

  return consumed;
}

void OSView::draw(U8G2 *u8g2) {
  // draw all child views
  for (int i = 0; i < MAX_CHILD_VIEWS; i++) {
    if (_childViews[i] != NULL) {
      _childViews[i]->draw(u8g2);
    }
  }
}
