#ifndef DEBUG
# include <BuildInfo.h>
#else // ifdef EXT_BUILD
# define BUILD_HASH "DEBUG"
# define BUILD_DATE "0000-00-00"
# define BUILD_TIME "00:00:00"
#endif // ifdef EXT_BUILD

#include "Arduino.h"
#include <BME280I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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
#include <OSViewCycle.h>
#include <OSSensors.h>
#include <OSTimers.h>

#include <TextViews.h>
#include <ChartViews.h>

#include <IoDClient.h>
#include <IoDAuth.h>

// Variables required for the display
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0,

                                            /* clock=*/ D0,

                                            /* data=*/ D5,

                                            /* cs=*/ D8,

                                            /* dc=*/ D7,

                                            /* reset=*/ D6);

OSTime osTime;
OSTimers *timers;

OSButtons buttons;
OSSensors sensors(true, 1);

OSViewCycle *rootCycle, *tempCycle, *humCycle, *presCycle;
OSView *overlays;

LineChart *tempLineChart, *humLineChart, *presLineChart;

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

char *lblBuildHash = str2char(BUILD_HASH);
char *lblBuildDate = str2char(BUILD_DATE);
char *lblBuildTime = str2char(BUILD_TIME);

char *lblBtnTime  = str2char("⏲↻▶");
char *lblWait     = str2char("⏳");
char *lblButtonD3 = str2char("▶");
char *lblButtonD4 = str2char("▼");
char *lblTime     = str2char("15:27:00");
char *lblUpTime   = str2char("00:00:00");
char *lblWifiSSID = str2char(WIFI_SSID);

char *lblSUpTime = str2char("Uptime");
char *lblSTime   = str2char("Time");

char *lblTemp = new char[8];
char *lblPres = new char[8];
char *lblHum  = new char[8];
char *lblAlt  = new char[8];

char *lblIp = str2char("000.000.000.000");

IoDClient iodClient(IOD_HOST, IOD_PORT, IOD_USER, IOD_PASS, IOD_SSL_FINGERPRINT);

void tmrDraw() {
  osTime.getUptimeStr(lblUpTime);
  osTime.getUnixTimeStr(lblTime);

  float2char(sensors.getTemp(), 1, lblTemp);
  float2char(sensors.getPres(), 1, lblPres);
  float2char(sensors.getHum(),  1, lblHum);

  lblSignalStrength = getWifiStrength();
  String ip = String(WiFi.localIP()[0])
              + "." + String(WiFi.localIP()[1])
              + "." + String(WiFi.localIP()[2])
              + "." + String(WiFi.localIP()[3]);
  strcpy(lblIp, ip.c_str());

  u8g2.clearBuffer();
  rootCycle->draw(&u8g2);
  u8g2.sendBuffer();
}

void tmrButtonClicks() {
  buttons.handleButtonClicks();
}

void tmrPrintHeap() {
  Serial.println(ESP.getFreeHeap());
}

void tmrReadSensors() {
  sensors.readValues();
  tempLineChart->addValue(sensors.getTemp());
  humLineChart->addValue(sensors.getHum());
  presLineChart->addValue(sensors.getPres());
}

void tmrIoDClient() {
  if (WiFi.status() == WL_CONNECTED) {
    String ids[] = { IOD_NODE_TEMP,
                     IOD_NODE_HUM,
                     IOD_NODE_BARO };
    String values[] = { String(sensors.getTemp(),  1),
                        String(sensors.getHum(),  1),
                        String(sensors.getPres(), 1) };
    iodClient.postMulti(ids, values, 3);
    delay(3000); // I think this helps. Classic programming.
  }
}

bool clickRootCycle(int id) {
  bool ret = rootCycle->click(id);

  tmrDraw();
  return ret;
}

bool clickUpdateTime(int id) {
  u8g2.clearBuffer();
  CornerText t(&lblWait, TOP_LEFT);
  t.draw(&u8g2);
  u8g2.sendBuffer();

  WiFiClient client;

  osTime.setUnixTime(webUnixTime(client) + (2 * 60 * 60)); // UTC +2
  return true;
}

#include <index.html.h>

ESP8266WebServer server(80);

void handle_index() {
  Serial.print("HTTP: / ");
  server.send(200, "text/html", create_index_html());
  Serial.println("200 OK");
}

void setup(void) {
  // Setup interfaces/hardware

  Serial.begin(9600);

  pinMode(D3, INPUT);
  digitalWrite(D3, HIGH); // turn on pullup resistors
  pinMode(D4, INPUT);
  digitalWrite(D4, HIGH); // turn on pullup resistors

  u8g2.begin();
  u8g2.setContrast(0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Setup software

  rootCycle = new OSViewCycle(D3);
  tempCycle = new OSViewCycle(D4);
  humCycle  = new OSViewCycle(D4);
  presCycle = new OSViewCycle(D4);

  overlays = new OSView();
  overlays->addView(new CornerText(&lblSignalStrength, TOP_LEFT));
  overlays->addView(new CornerText(&lblButtonD4, BOTTOM_RIGHT));

  rootCycle->addView((new LargeUnitText(&lblUpTime, &lblEmpty))
                     ->addView(new BorderText(&lblSUpTime, BOTTOM))
                     ->addView(overlays)
                     );
  rootCycle->addView((new LargeUnitText(&lblTime, &lblTimeUnit))
                     ->addView(new BorderText(&lblSTime, BOTTOM))
                     ->addView(new CornerText(&lblBtnTime, BOTTOM_RIGHT))
                     ->addView(overlays));
  rootCycle->addView(tempCycle);
  rootCycle->addView(humCycle);
  rootCycle->addView(presCycle);
  TextView *infoView = new TextView(&lblIp);
  infoView->setOffsetX(0);
  infoView->setOffsetY(31);
  rootCycle->addView(infoView //
                     ->addView(new CornerText(&lblBuildDate, TOP_RIGHT))
                     ->addView(new CornerText(&lblBuildTime, BOTTOM_RIGHT))
                     ->addView(new CornerText(&lblBuildHash, BOTTOM_LEFT))
                     ->addView(overlays));
  rootCycle->addView(new LargeText(&lblEmpty));


  // overlays->addView(new CornerText(&lblWifiSSID, TOP_RIGHT));
  tempCycle->setOverlay(overlays);
  humCycle->setOverlay(overlays);
  presCycle->setOverlay(overlays);

  tempCycle->addView(new LargeUnitText(&lblTemp, sensors.getTempUnit()));
  humCycle->addView(new LargeUnitText(&lblHum, sensors.getHumUnit()));
  presCycle->addView(new LargeUnitText(&lblPres, sensors.getPresUnit()));

  tempLineChart = new LineChart(0, 16, 128, 48, 4.0);
  tempLineChart->addView(new CornerText(&lblTemp, TOP_RIGHT));
  tempCycle->addView(tempLineChart);

  humLineChart = new LineChart(0, 16, 128, 48, 10.0);
  humLineChart->addView(new CornerText(&lblHum, TOP_RIGHT));
  humCycle->addView(humLineChart);

  presLineChart = new LineChart(0, 16, 128, 48, 10.0);
  presLineChart->addView(new CornerText(&lblPres, TOP_RIGHT));
  presCycle->addView(presLineChart);

  sensors.setup();
  buttons.setup(5);
  buttons.registerButtonClick(D3, &clickRootCycle);
  buttons.registerButtonClick(D4, &clickRootCycle);
  buttons.registerButtonClick(D4, &clickUpdateTime);

  timers = new OSTimers();
  timers->registerTimer(new OSTimer(&tmrButtonClicks, 120));
  timers->registerTimer(new OSTimer(&tmrDraw, 500));
  timers->registerTimer(new OSTimer(&tmrReadSensors, 2500));
  timers->registerTimer(new OSTimer(&tmrPrintHeap, 5000));
  timers->registerTimer(new OSTimer(&tmrIoDClient, 15 * 60 * 1000));

  server.on("/", handle_index);
  server.begin();
}

void loop(void) {
  unsigned long diff = osTime.tick();

  timers->checkTimers(diff);

  server.handleClient();

  delay(100);
}
