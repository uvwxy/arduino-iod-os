#ifndef OSButtons_h
#define OSButtons_h

// Helpers to map to Arduino ports,
// as the ESP8266 pins are a little bit confusing
#define D0 16
#define D1 5  // I2C Bus SCL (clock)
#define D2 4  // I2C Bus SDA (data)
#define D3 0
#define D4 2  // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3  // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#define MAX_BUTTON_CALLBACKS 16
#define MAX_BUTTONS 2

class OSButtons {
public:

  void setup(int clickTimeout);
  int  registerButtonClick(int    pin,
                           bool (*callback)(int));
  void handleButtonClicks();

protected:

  void handleClick(int pin,
                   int index);

private:

  int _buttonMap[MAX_BUTTONS];
  int _buttonLastState[MAX_BUTTONS];
  int _clickTimeout = 3; // ticks
  bool(*_buttonCallbacks[MAX_BUTTONS][MAX_BUTTON_CALLBACKS])(int) = {};
};

#endif // ifndef Buttons_h
