# Internet of Dingens Operating System

## Click for Video
[![IOD OS](https://img.youtube.com/vi/h6Gr7AzSxl0/0.jpg)](https://www.youtube.com/watch?v=h6Gr7AzSxl0)

## Building

1. Install [Atom](https://atom.io) and [PlatformIO](http://platformio.org).
2. Checkout this repository
    1. `cd src`
    2. `cp WiFiAuth.h.example WiFiAuth.h` and edit the `WIFI_SSID`, `WIFI_PASS` definitions

Also don't forget to install the CH340g driver incase you have this USB chip instead of the FTDI to connect the ESP8266.

See [here](http://kig.re/2014/12/31/how-to-use-arduino-nano-mini-pro-with-CH340G-on-mac-osx-yosemite.html).

## Description

This project is

- hosted on GitHub as a backup 😎
- undocumented 😱
- requires special ESP8266 based hardware 😵
- far from finished 🤓
- my playground to learn C++ 🤡
- a lot of fun 😎👍
- still a little buggy

My idea is

- create tiny sensors (temperature, humidity, pressure), and log data on a RaspberryPi, like many others have already
- to write some reusable code with C++
- reuse this code (*mainly the OSViews*) with multiple projects
- publish hardware design
- publish backend
- document everything
- have fun and learn


What works

- display temperature, pressure, humidity (OSSensors.h)
- simple UI to display uptime, time, sensors (OSView.h, TextViews.h, OSViewCycle.h)
- proto threading (OSTimers.h)
- button clicks (OSButtons.h)
- time keeping (OSTime.h)
