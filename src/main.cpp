#include "Arduino.h"
#include <BME280I2C.h>
#include <ESP8266WiFi.h>

#include <WifiAuth.h>
#include <WebTime.h>

// The display (SSD1306) is connected via SPI
#include <Wire.h>
#include <SPI.h>
#include <U8g2lib.h>

#include <OSHelper.h>
#include <OSTime.h>
#include <OSButtons.h>
#include <OSView.h>
#include <OSTextViews.h>
#include <OSViewCycle.h>
#include <OSSensors.h>

// Variables required for the display
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0,

                                            /* clock=*/ D0,

                                            /* data=*/ D5,

                                            /* cs=*/ D8,

                                            /* dc=*/ D7,

                                            /* reset=*/ D6);

OSTime time;
OSButtons buttons;
OSSensors sensors(true, 1);

OSViewCycle *rootView, *sensorViews;
OSView *overlays;

LargeUnitText *upTimeView, *tempView, *humView, *presView, *heightView, *timeView;

char *lblEmpty = new char[1] { 0 };
char *lblTimeUnit = str2char("⏲");

char *lblSignalStrengths[] = { str2char("○○○○○"), str2char("●○○○○"), str2char("●●○○○"), str2char("●●●○○"), str2char("●●●●○"), str2char("●●●●●") };
char *lblSignalStrength = lblSignalStrengths[0];
char* getWifiStrength() {
  if (!WiFi.isConnected()) {
    return lblSignalStrengths[0];
  }

  int rssiPercent = 2 * (WiFi.RSSI() + 100);
  rssiPercent = rssiPercent < 0 ? 0 : rssiPercent;
  rssiPercent = rssiPercent > 100 ? 100 : rssiPercent;
  int index = 1 + (rssiPercent / 20);

  return lblSignalStrengths[index];
}

char *lblBtnTime   = str2char("⏲↻▶");
char *lblWait      = str2char("⏳");
char *lblButtonD3  = str2char("▶");
char *lblButtonD4  = str2char("▼");
char *lblPresTrend = str2char("☀");
char *lblTime      = str2char("15:27:00");
char *lblUpTime    = str2char("00:00:00");
char *lblWifiSSID  = str2char(WIFI_SSID);

char *lblSUptime = str2char("Uptime");
char *lblSTime   = str2char("Time");

char *lblTemp = new char[8];
char *lblPres = new char[8];
char *lblHum  = new char[8];
char *lblAlt  = new char[8];


bool clickRootCycle(int id) {
  return rootView->click(id);
}

bool clickUpdateTime(int id) {
  u8g2.clearBuffer();
  CornerText t(&lblWait, TOP_LEFT);
  t.draw(&u8g2);
  u8g2.sendBuffer();

  WiFiClient client;

  time.setUnixTime(webUnixTime(client) + (2 * 60 * 60));
  Serial.println(time.getUnixTime());
  return true;
}

void setup(void) {
  Serial.begin(9600);

  u8g2.begin();
  u8g2.setContrast(0);

  overlays = new OSView();
  overlays->addView(new CornerText(&lblSignalStrength, TOP_LEFT));
  overlays->addView(new CornerText(&lblWifiSSID, TOP_RIGHT));
  overlays->addView(new CornerText(&lblButtonD4, BOTTOM_RIGHT));
  overlays->addView(new CornerText(&lblPresTrend, BOTTOM_LEFT));

  // overlays->addView(new BorderText(&lblUpTime, TOP));
  // overlays->addView(new BorderText(new String("(1)"), RIGHT));
  // overlays->addView(new BorderText(new String("(2)"), BOTTOM));
  // overlays->addView(new BorderText(new String("(3)"), LEFT));

  upTimeView = new LargeUnitText(&lblUpTime, &lblEmpty);
  tempView   = new LargeUnitText(&lblTemp, sensors.getTempUnit());
  humView    = new LargeUnitText(&lblHum, sensors.getHumUnit());
  presView   = new LargeUnitText(&lblPres, sensors.getPresUnit());

  sensorViews = new OSViewCycle(D3);
  sensorViews->setOverlay(overlays);

  upTimeView->addView(new BorderText(&lblSUptime, BOTTOM));
  sensorViews->addView(upTimeView);

  LargeUnitText *tv = new LargeUnitText(&lblTime, &lblTimeUnit);
  tv->addView(new BorderText(&lblSTime, BOTTOM));
  sensorViews->addView(tv);

  sensorViews->addView(tempView);
  sensorViews->addView(humView);
  sensorViews->addView(presView);

  rootView = new OSViewCycle(D4);

  rootView->addView(sensorViews);
  timeView = new LargeUnitText(&lblTime, &lblTimeUnit);
  timeView->addView(new CornerText(&lblBtnTime, TOP_RIGHT));
  rootView->addView(timeView);
  rootView->addView(new LargeText(&lblEmpty));

  sensors.setup();
  buttons.setup();
  buttons.registerButtonClick(D3, &clickRootCycle);
  buttons.registerButtonClick(D4, &clickRootCycle);
  buttons.registerButtonClick(D3, &clickUpdateTime);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void loop(void) {
  time.tick();
  time.getUptimeStr(lblUpTime);
  time.getUnixTimeStr(lblTime);

  sensors.readValues();
  float2char(sensors.getTemp(), 1, lblTemp);
  float2char(sensors.getPres(), 1, lblPres);
  float2char(sensors.getHum(),  1, lblHum);

  lblSignalStrength = getWifiStrength();

  buttons.handleButtonClicks();

  u8g2.clearBuffer();

  rootView->draw(&u8g2);
  u8g2.sendBuffer();

  Serial.println(ESP.getFreeHeap());

  delay(1000);
}
