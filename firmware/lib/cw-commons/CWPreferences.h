#pragma once

#include <Preferences.h>

Preferences preferences;

struct ClockwiseParams
{
    //these should match the html DOM ids in the forms in HTML
    const char* const PREF_SWAP_BLUE_GREEN = "swapBlueGreen";
    const char* const PREF_USE_24H_FORMAT = "use24hFormat";
    const char* const PREF_DISPLAY_BRIGHT = "displayBright";
    const char* const PREF_TIME_ZONE = "timeZone";
    const char* const PREF_WIFI_SSID = "wifiSsid";
    const char* const PREF_WIFI_PASSWORD = "wifiPwd";
    const char* const PREF_SYMBOL_1 = "symbol1";
    const char* const PREF_SYMBOL_2 = "symbol2";
    const char* const PREF_SYMBOL_3 = "symbol3";
    const char* const PREF_SYMBOL_4 = "symbol4";
    const char* const PREF_SYMBOL_5 = "symbol5";


    bool swapBlueGreen;
    bool use24hFormat;
    uint8_t displayBright;
    String timeZone;
    String wifiSsid;
    String wifiPwd;
    String symbol1;
    String symbol2;
    String symbol3;
    String symbol4;
    String symbol5;


    ClockwiseParams() {
        preferences.begin("clockwise", false); 
        //preferences.clear();
    }

    static ClockwiseParams* getInstance() {
        static ClockwiseParams base;
        return &base;
    }

   
    void save()
    {
        preferences.putBool(PREF_SWAP_BLUE_GREEN, swapBlueGreen);
        preferences.putBool(PREF_USE_24H_FORMAT, use24hFormat);
        preferences.putUInt(PREF_DISPLAY_BRIGHT, displayBright);
        preferences.putString(PREF_TIME_ZONE, timeZone);
        preferences.putString(PREF_WIFI_SSID, wifiSsid);
        preferences.putString(PREF_WIFI_PASSWORD, wifiPwd);
        preferences.putString(PREF_SYMBOL_1, symbol1);
        preferences.putString(PREF_SYMBOL_2, symbol2);
        preferences.putString(PREF_SYMBOL_3, symbol3);
        preferences.putString(PREF_SYMBOL_4, symbol4);
        preferences.putString(PREF_SYMBOL_5, symbol5);
    }

    void load()
    {
        swapBlueGreen = preferences.getBool(PREF_SWAP_BLUE_GREEN, false);
        use24hFormat = preferences.getBool(PREF_USE_24H_FORMAT, true);
        displayBright = preferences.getUInt(PREF_DISPLAY_BRIGHT, 32);
        timeZone = preferences.getString(PREF_TIME_ZONE, "America/Sao_Paulo");
        wifiSsid = preferences.getString(PREF_WIFI_SSID, "");
        wifiPwd = preferences.getString(PREF_WIFI_PASSWORD, "");
        symbol1 = preferences.getString(PREF_SYMBOL_1, "NQ=F");
        symbol2 = preferences.getString(PREF_SYMBOL_2, "ES=F");
        symbol3 = preferences.getString(PREF_SYMBOL_3, "YM=F");
        symbol4 = preferences.getString(PREF_SYMBOL_4, "^HSI");
        symbol5 = preferences.getString(PREF_SYMBOL_5, "0005.HK");
    }

};
