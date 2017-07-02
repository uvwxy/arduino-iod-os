
/*
  IoDClient.h - Library to send data to the IoD Backend
  Created by Paul Smith, March, 2017.
 */

#ifndef IoDClient_h
#define IoDClient_h

#include "Arduino.h"

class IoDClient {
public:
    IoDClient(const char *host, int port, const char *user, const char *pass, const char *sslFingerprint);

    void post(String id, String data);

private:
    const char *_host;
    int _port;
    const char *_user;
    const char *_pass;
    const char *_sslFingerprint;
};

#endif
