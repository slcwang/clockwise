#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// Clockface

#include <Clockface.h>
// Commons
#include <WiFiController.h>
#include <CWDateTime.h>

#include <CWWebServer.h>
#include <StatusController.h>
#include <CWPreferences.h>
MatrixPanel_I2S_DMA *dma_display = nullptr;

Clockface *clockface;

WiFiController wifi;
CWDateTime cwDateTime;

void displaySetup(bool swapBlueGreen, uint8_t displayBright)
{
  HUB75_I2S_CFG mxconfig(64, 64, 1);

  if (swapBlueGreen)
  {
    // Swap Blue and Green pins because the panel is RBG instead of RGB.
    mxconfig.gpio.b1 = 26;
    mxconfig.gpio.b2 = 12;
    mxconfig.gpio.g1 = 27;
    mxconfig.gpio.g2 = 13;
  }

  mxconfig.gpio.e = 18;
  mxconfig.clkphase = false;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(displayBright);
  dma_display->clearScreen();
}

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  StatusController::getInstance()->blink_led(5, 100);

  ClockwiseParams::getInstance()->load();

  displaySetup(ClockwiseParams::getInstance()->swapBlueGreen, ClockwiseParams::getInstance()->displayBright);
  clockface = new Clockface(dma_display);

  StatusController::getInstance()->clockwiseLogo();
  delay(1000);

  StatusController::getInstance()->wifiConnecting();
  if (wifi.begin())
  {
    StatusController::getInstance()->wifiConnected();
    delay(1000);
    StatusController::getInstance()->ntpConnecting();
    cwDateTime.begin(ClockwiseParams::getInstance()->timeZone.c_str(), ClockwiseParams::getInstance()->use24hFormat);
    clockface->setup(&cwDateTime, ClockwiseParams::getInstance()->symbol1, ClockwiseParams::getInstance()->symbol2, ClockwiseParams::getInstance()->symbol3, ClockwiseParams::getInstance()->symbol4, ClockwiseParams::getInstance()->symbol5);
  }
}

void loop()
{
  wifi.handleImprovWiFi();

  if (WiFiController::isConnected())
  {
    ClockwiseWebServer::getInstance()->handleHttpRequest();
  }

  if (wifi.connectionSucessfulOnce)
  {
    clockface->update();
  }
}
