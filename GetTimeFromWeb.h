#ifndef __GETTIMEFROMWEB_H
#define __GETTIMEFROMWEB_H
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ctime> 
#include <Preferences.h>  

void wifi_ntpinit(const char * ssid,const char *password);//这个函数用于时钟联网授时相关程序的初始化，在MyHtmlMenu中被初始化，无需再主程序（Mymenu）中再次初始化



int GetTime();      //从网络中获取时间(要确保这个函数包含在loop中)



int GetTime_hour();
int GetTime_min();
int GetTime_sec();



int GetTime_date(int a);


int Situation_Of_Wifi();            //若返回值为1则wifi当前状态是链接成功，其他状态返回0
String Getssid();
#endif