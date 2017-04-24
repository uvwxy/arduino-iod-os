#ifndef OSTextViews_h
#define OSTextViews_h

#include <OSView.h>

class TextView : public OSView {
public:

  TextView(char **str);
  virtual void draw(U8G2 *u8g2) {}

  void         drawUTF8Str(U8G2 *u8g2,
                           int   x,
                           int   y,
                           char *str);
  void setOffsetX(int x);
  void setOffsetY(int y);

protected:

  int offsetX = 0, offsetY = 0;
  char **_str;
};

class LargeText : public TextView {
public:

  LargeText(char **str);
  void draw(U8G2 *u8g2);
};

class LargeUnitText : public TextView {
public:

  LargeUnitText(char **str,
                char **unit);
  void draw(U8G2 *u8g2);

private:

  char **_unit;
};

enum CornerGravity { TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT };

class CornerText : public TextView {
public:

  CornerText(char        **str,
             CornerGravity g);
  void draw(U8G2 *u8g2);

private:

  CornerGravity _g;
};

enum BorderGravity { TOP, RIGHT, BOTTOM, LEFT };

class BorderText : public TextView {
public:

  BorderText(char        **str,
             BorderGravity g);
  void draw(U8G2 *u8g2);

private:

  BorderGravity _g;
};

#endif // ifndef OSTextViews_h
