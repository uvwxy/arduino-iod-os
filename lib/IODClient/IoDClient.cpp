#include "Arduino.h"
#include "IoDClient.h"

/*
 * IMPORTANT:
 * This client assumes the WiFi is already connected. Also the following
 */

#include <WiFiClientSecure.h>
#include <base64.h>

IoDClient::IoDClient(const char *host,
                     int         port,
                     const char *user,
                     const char *pass,
                     const char *sslFingerprint) {
  _host           = host;
  _port           = port;
  _user           = user;
  _pass           = pass;
  _sslFingerprint = sslFingerprint;
}

void send(const char *host,
          int         port,
          String      user,
          String      pass,
          String      url,
          const char *sslFingerprint) {
  WiFiClientSecure client;

  if (!client.connect(host, port)) {
    #ifndef DEBUG
    Serial.println("IoDClient not connected");
    #endif // ifndef DEBUG
    return;
  }

#ifdef DEBUG

  if (client.verify(sslFingerprint, host)) {
    // TODO
  } else {
    // TODO
  }
#endif // ifndef DEBUG

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: IODNodeESP8266\r\n" +
               "Authorization: Basic " + base64::encode(
                 user + ":" + pass) + "\r\n" +
               "Connection: close\r\n\r\n");

#ifdef DEBUG
  Serial.println("request sent");

  int count = 0;

  while (client.connected()) {
    count++;
    String line = client.readStringUntil('\n');
    Serial.println(line);

    if (line == "\r") {
# ifdef DEBUG
      Serial.println("headers received");
# endif // ifdef DEBUG
      break;
    }

    if (count == 12) {
# ifdef DEBUG
      Serial.println("max reads from server reached, canceling");
# endif // ifdef DEBUG
      break;
    }
  }
  Serial.println(client.readStringUntil('\n'));
#endif // ifndef DEBUG

  client.stop();
}

void IoDClient::post(String id, String value) {
  String url = "/api/value/";

  url += id;
  url += "/0/"; // server based timestamp
  url += value;

  send(_host, _port, _user, _pass, url, _sslFingerprint);
}

void IoDClient::postMulti(String ids[], String values[], int n) {
  String id   = "";
  String data = "";
  int    i    = 0;

  for (i = 0; i < n; i++) {
    if (i > 0) {
      id += ",";
    }
    id += ids[i];
  }

  for (i = 0; i < n; i++) {
    if (i > 0) {
      data += ",";
    }
    data += values[i];
  }

  String url = "/api/values/";
  url += id;
  url += "/0/"; // server based timestamp
  url += data;

  send(_host, _port, _user, _pass, url, _sslFingerprint);
}
