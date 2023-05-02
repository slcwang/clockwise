//#include <CWPreferences.h>
#include "Clockface.h"
#include "HTTPClient.h"
#include <ArduinoJson.h>
//#include <CWPreferences.h>

unsigned long lastMillis = 0;

Clockface::Clockface(Adafruit_GFX* display)
{
  _display = display;
  Locator::provide(display);
}

void Clockface::setup(CWDateTime *dateTime, String symbol1, String symbol2, String symbol3, String symbol4, String symbol5 ) {
  this->_dateTime = dateTime;
  this->_symbol1 = symbol1;
  this->_symbol2 = symbol2;
  this->_symbol3 = symbol3;
  this->_symbol4 = symbol4;
  this->_symbol5 = symbol5;
  // Clear screen
  Locator::getDisplay()->fillRect(0, 0, 64, 64, 0x0000);

  // Set font (Picopixel is present in commons package)
  //Locator::getDisplay()->setFont(&Picopixel);
  Locator::getDisplay()->setFont();
  // Write on display
  //Locator::getDisplay()->setCursor(3, 3);
  //Locator::getDisplay()->print("Clockwise");
  
  // Text color

  Locator::getDisplay()->setTextColor(0xFFFF);

  // Draw Bitmaps (also present in commons package)
  //Locator::getDisplay()->drawRGBBitmap(3, 23, MAIL, 8, 8);
  //Locator::getDisplay()->drawRGBBitmap(15, 22, WEATHER_CLOUDY_SUN, 8, 8);

  // Draw date (it's also used in loop())
  refreshDate();

  // Clear font (now using the default one)
  Locator::getDisplay()->setFont();

  drawStocks();
}


void Clockface::update(){  
  if (millis() - lastMillis >= 1000) {

    snprintf(time, sizeof(time), "%d:%02d:%02d", _dateTime->getHour(), _dateTime->getMinute(), _dateTime->getSecond());

    // Used for testing
    // snprintf(time, sizeof(time), "%d:%d:%d", 20, 45, 15);

    // Draw a black rect to clean the area
    Locator::getDisplay()->fillRect(31, 50, 33, 8, 0x0000);
    Locator::getDisplay()->setFont(&Picopixel);
    Locator::getDisplay()->setTextColor(0xFFFF);//(random(LONG_MAX));
    Locator::getDisplay()->setCursor(34, 55);
    Locator::getDisplay()->print(time);

    //if (_dateTime->getMinute() == 0 && _dateTime->getSecond() == 0) {  //every hour
    if ( _dateTime->getMinute()%5 == 0 && _dateTime->getSecond() == 0) { //every 5min
      refreshDate();

      drawStocks();
    }

    lastMillis = millis();
  }  

}

void Clockface::refreshDate() {
  Locator::getDisplay()->setFont(&Picopixel);

  snprintf(date, sizeof(date), "%02d/%02d", _dateTime->getMonth(), _dateTime->getDay());
  
  //snprintf(date, sizeof(date), "%d/%d", 12, 25);
  Locator::getDisplay()->fillRect(0, 48, 25, 7, 0x0000);// clear black
  Locator::getDisplay()->fillRect(3, 50, 25, 7, 0xFFFF);// create white box
  Locator::getDisplay()->setTextColor(0x0000);
  Locator::getDisplay()->setCursor(6, 55);
  Locator::getDisplay()->print(date);
  
  Locator::getDisplay()->setFont();
}

String Clockface::httpGET(String httpPath){
  HTTPClient http;
  //String serverPath = serverName + "?temperature=24.37";
  String serverPath = httpPath;
  // Your Domain name with URL path or IP address with path
  http.begin(serverPath.c_str());
  
  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    return payload;

  }else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  //String noResponse = 
  return "{\"chart\":{\"result\":[{\"meta\":{\"regularMarketPrice\":0.0,\"chartPreviousClose\":0.0,\"priceHint\":2}}]}}";
  //return "no response";
}

void Clockface::drawStocks() {
  drawLine(1,_symbol1);//"^HSI");//ClockwiseParams::getInstance()->PREF_SYMBOL_1);
  drawLine(2,_symbol2);//"NQ=F");//ClockwiseParams::getInstance()->PREF_SYMBOL_2);
  drawLine(3,_symbol3);//"ES=F");//ClockwiseParams::getInstance()->PREF_SYMBOL_3);
  drawLine(4,_symbol4);//"YM=F");//ClockwiseParams::getInstance()->PREF_SYMBOL_4);
  drawLine(5,_symbol5);//"0005.HK");//ClockwiseParams::getInstance()->PREF_SYMBOL_5);
  //drawLine(5,ClockwiseParams::getInstance()->PREF_SYMBOL_5);
}

void Clockface::drawLine(int line, String stock) {
  int lineHeight = 9;
  String path = "https://query1.finance.yahoo.com/v8/finance/chart/";
  path = path + stock;
  path = path + "?interval=1d&range=1d";
  //httpGET(path);
  DynamicJsonDocument doc1(2048);
  deserializeJson(doc1, httpGET(path));
  double tickerPrice1 = doc1["chart"]["result"][0]["meta"]["regularMarketPrice"];//  chart.result[0].meta.regularMarketPrice;
  double tickerPricePrev1 = doc1["chart"]["result"][0]["meta"]["chartPreviousClose"];
  if(tickerPrice1 == 0) return; //0 means http get failed, so skip all the drawing below.

  if(tickerPrice1 < tickerPricePrev1){
    Locator::getDisplay()->setTextColor(0xF800); //red
  }else if(tickerPrice1 >= tickerPricePrev1){
    Locator::getDisplay()->setTextColor(0x07E1); //green
  }else{
    Locator::getDisplay()->setTextColor(0xEEEE); //White
  }
  
  Locator::getDisplay()->fillRect(0, line*lineHeight-5, 63, lineHeight, 0x0000); //clear screen
  Locator::getDisplay()->setFont(&Picopixel);
  Locator::getDisplay()->setCursor(3, line*lineHeight);
  stock.replace(".HK","");
  stock.replace("-USD","");
  stock.replace("^","");
  stock.replace("=F","");
  Locator::getDisplay()->print(stock);

  int roundTo = 0;
  if(tickerPrice1/1000 >= 1){
    roundTo = 0;
  }else if(tickerPrice1/100 >= 1){
    roundTo = 1;
  }else if(tickerPrice1/10 >= 1){
    roundTo = 2;
  }else /*if(tickerPrice1/1 >= 1)*/{
    roundTo = 3;
  }/*else {
    roundTo = 4;
  }*/

  Locator::getDisplay()->setCursor(20, line*lineHeight); //price
  Locator::getDisplay()->print((tickerPrice1),roundTo);
  Locator::getDisplay()->setCursor(46, line*lineHeight); 
  if(tickerPrice1 >= tickerPricePrev1){
    Locator::getDisplay()->print("+");  // draw "+"
    Locator::getDisplay()->setCursor(50, line*lineHeight);  // shift curser
  }

  //Locator::getDisplay()->print(round(tickerPrice1-tickerPricePrev1),roundTo); //delta

  Locator::getDisplay()->print(100*(tickerPrice1-tickerPricePrev1)/tickerPricePrev1,1); //percentage
  Locator::getDisplay()->setCursor(60, line*lineHeight);
  Locator::getDisplay()->print("%");
 
}
