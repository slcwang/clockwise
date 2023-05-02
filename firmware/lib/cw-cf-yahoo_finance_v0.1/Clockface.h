#pragma once

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Tile.h>
#include <Locator.h>
#include <Game.h>
#include <Object.h>
#include <ImageUtils.h>
#include <WiFi.h>

// Commons
#include "IClockface.h"
#include "Icons.h"
#include "picopixel.h"



class Clockface: public IClockface {
  private:
    Adafruit_GFX* _display;
    CWDateTime* _dateTime;
    String _symbol1;
    String _symbol2;
    String _symbol3;
    String _symbol4;
    String _symbol5;
    char time[9] = {0};
    char date[6] = {0};
    String httpGET(String httpPath);
    void drawLine(int line, String stock);
    void drawStocks();
  public:
    Clockface(Adafruit_GFX* display);
    void setup(CWDateTime *dateTime, String symbol1, String symbol2, String symbol3, String symbol4, String symbol5 );
    void update();
    void refreshDate();
};
