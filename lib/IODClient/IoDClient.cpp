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

void IoDClient::post(String id, String data) {
  WiFiClientSecure client;

  if (!client.connect(_host, _port)) {
#ifndef DEBUG
    Serial.println("client not connected");
#endif // ifndef DEBUG
    return;
  }

  String url = "/api/value/";
  url += id;
  url += "/0/"; // server based timestamp
  url += data;

#ifndef DEBUG

  if (client.verify(_sslFingerprint, _host)) {
    // TODO
  } else {
    // TODO
  }
#endif // ifndef DEBUG

  String auth = _user;
  auth += ":";
  auth += _pass;


  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + _host + "\r\n" +
               "User-Agent: IODNodeESP8266\r\n" +
               "Authorization: Basic " + base64::encode(auth) + "\r\n" +
               "Connection: close\r\n\r\n");

#ifndef DEBUG
  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);

    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println(client.readStringUntil('\n'));
#endif // ifndef DEBUG
  client.stop();
}
