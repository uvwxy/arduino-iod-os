#ifndef index_html_h
#define index_html_h
#include <Arduino.h>
String create_index_html(){
  String ret = "";
  ret +="<html>";
  ret +="<head>";
  ret +="<title>IoD-Node: Sensors</title>";
  ret +="</head>";
  ret +="<body>";
  ret +="<div>";
  ret +="<h2>Status</h2>";
  ret +="<ul>";
  ret +="<li>Uptime:&nbsp;<b>";
  ret += lblUpTime;
  ret +="</b></li>";
  ret +="<li>Time:&nbsp;<b>";
  ret += lblTime;
  ret +="</b></li>";
  ret +="</ul>";
  ret +="<h2>Sensors</h2>";
  ret +="<ul>";
  ret +="<li>Temp.:&nbsp;<b>";
  ret += lblTemp;
  ret +="</b></li>";
  ret +="<li>Hum.:&nbsp;<b>";
  ret += lblHum;
  ret +="</b></li>";
  ret +="<li>Pres.:&nbsp;<b>";
  ret += lblPres;
  ret +="</b></li>";
  ret +="</ul>";
  ret +="</div>";
  ret +="</body>";
  ret +="</html>";
  return ret;
}
#endif
