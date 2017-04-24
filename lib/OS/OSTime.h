#ifndef OSTime_h
#define OSTime_h

class OSTime {
public:

  OSTime();

  unsigned long getUptimeSeconds();
  void          getUptimeStr(char *& str);
  void          getUnixTimeStr(char *& str);
  void          get24hStr(char *& str,
                          int     seconds);
  unsigned long getUnixTime();
  void          setUnixTime(unsigned long t);
  void          tick();

private:

  unsigned long millisLast;
  unsigned long uptimeSeconds;
  unsigned long unixtime;
  char *uptimeStr;
};


#endif // ifndef OSTime_h
