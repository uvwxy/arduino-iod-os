#include <OSTextViews.h>

TextView::TextView(char **str) : OSView() {
  _str = str;
}

void TextView::drawUTF8Str(U8G2 *u8g2, int x, int y, char *str) {
  u8g2->drawUTF8(x + offsetX, y + offsetY, str);
}

void TextView::setOffsetX(int x) {
  offsetX = x;
}

void TextView::setOffsetY(int y) {
  offsetY = y;
}

LargeText::LargeText(char **str) : TextView(str) {}

void LargeText::draw(U8G2 *u8g2) {
  u8g2->setFont(u8g2_font_profont29_mr);
  int strWidth  = u8g2->getStrWidth(*_str);
  int strHeight = 29;

  TextView::drawUTF8Str(u8g2, DISPLAY_WIDTH / 2 - (strWidth / 2),
                        (DISPLAY_HEIGHT / 2) + (strHeight / 2), *_str);


  OSView::draw(u8g2);
}

LargeUnitText::LargeUnitText(char **str, char **unit) : TextView(str) {
  _unit = unit;
}

void LargeUnitText::draw(U8G2 *u8g2) {
  u8g2->setFont(u8g2_font_profont29_mn);

  int strWidth  = u8g2->getStrWidth(*_str);
  int strHeight = 26;

  int x = DISPLAY_WIDTH / 2 - (strWidth / 2);
  int y = (DISPLAY_HEIGHT / 2) + (strHeight / 2);
  TextView::drawUTF8Str(u8g2, x, y, *_str);

  u8g2->setFont(u8g2_font_profont12_mr);
  TextView::drawUTF8Str(u8g2, x  + strWidth + 2, y, *_unit);

  OSView::draw(u8g2);
}

CornerText::CornerText(char **str, CornerGravity g) : TextView(str) {
  _g = g;
}

void CornerText::draw(U8G2 *u8g2) {
  u8g2->setFont(u8g2_font_unifont_t_symbols);
  int strWidth  = u8g2->getUTF8Width(*_str);
  int strHeight = 16;

  int x;
  int y;

  switch (_g) {
  case TOP_LEFT:
    x = 0;
    y = strHeight;
    break;

  case TOP_RIGHT:
    x = DISPLAY_WIDTH - strWidth;
    y = strHeight;
    break;

  case BOTTOM_LEFT:
    x = 0;
    y = DISPLAY_HEIGHT;
    break;

  case BOTTOM_RIGHT:
    x = DISPLAY_WIDTH - strWidth;
    y = DISPLAY_HEIGHT;
    break;
  }

  TextView::drawUTF8Str(u8g2, x, y, *_str);

  OSView::draw(u8g2);
}

BorderText::BorderText(char **str, BorderGravity g) : TextView(str) {
  _g = g;
}

void BorderText::draw(U8G2 *u8g2) {
  u8g2->setFont(u8g2_font_unifont_t_symbols);
  int strWidth  = u8g2->getUTF8Width(*_str);
  int strHeight = 16;

  int x;
  int y;

  switch (_g) {
  case TOP:
    x = (DISPLAY_WIDTH - strWidth) / 2;
    y = strHeight;
    break;

  case RIGHT:
    x = DISPLAY_WIDTH - strWidth;
    y = (DISPLAY_HEIGHT + strHeight) / 2;
    break;

  case BOTTOM:
    x = (DISPLAY_WIDTH - strWidth) / 2;
    y = DISPLAY_HEIGHT;
    break;

  case LEFT:
    x = 0;
    y = (DISPLAY_HEIGHT + strHeight) / 2;
    break;
  }

  TextView::drawUTF8Str(u8g2, x, y, *_str);

  OSView::draw(u8g2);
}
