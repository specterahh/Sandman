#ifndef __MYHTMLMENU_H
#define __MYHTMLMENU_H
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <GetTimeFromWeb.h>
#include <Preferences.h>

extern Preferences ap_prefs;          // 声明为外部变量
extern const char* default_ap_ssid;   // 声明为外部常量
extern const char* default_ap_password;

void MyHtmlMenuSetup();
String GetTimeTable(String WeekDay,String AMPM);
#endif