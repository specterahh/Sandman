#ifndef __MyClock_H
#define __MyClock_H
#include <Arduino.h>
#include <MyScreen.h>
#include <ESP32Time.h>
#include <GetTimeFromWeb.h>     
void MyClock_nint();                            //初始化时钟
void Myclock_output();                          //输出当前时间
void set_time(int sc, int mn, int hr, int dy, int mt, int yr);  //设置当前时间
int get_time(String lable) ;                    //lable =sc,mn,hr,dy,mt,yr根据不同的参数分别返回
String getAPM();                                //获取当前时间是上午还是下午
void Auto_Set_Time();                           //当且仅当WiFi链接成功时启用ntp为rtc校准时间
String getWeekDay();                            //返回当前是星期几，在自动话设置闹钟的模块有被调用
#endif