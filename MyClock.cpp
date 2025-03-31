#include <Arduino.h>
#include <MyScreen.h>
#include <ESP32Time.h>
#include <GetTimeFromWeb.h>                           //用于从网络上获取时间  
#include <MyTimeset.h>
ESP32Time rtc;                //创建一个 ESP32Time 对象 rtc，用于访问和操作 ESP32 的内部 RTC
String WeekDay;

void MyClock_nint()
{
    //Myscreen_init(0x3c);       //初始化屏幕
    //Myscreen_clear();          //清屏

    //初始化串口通信
    Serial.begin(115200);                   //初始化串口通信，波特率设置为 115200
    Serial.println("ESP32Time UP");         //在串口打印一条消息，表示程序已经启动

    //设置时间
    //设置时间的格式：setTime(int sc, int mn, int hr, int dy, int mt, int yr, int ms = 0)
     rtc.setTime(0, 56, 22,23, 1, 2025); // 设置时间为 2025 年 1 月 25 日 12:00:00

    

}


void set_time(int sc, int mn, int hr, int dy, int mt, int yr)           //用于部设置当前rtc的时间，在MyTimeset.cpp中有被调用
{

  rtc.setTime(sc, mn, hr,dy, mt, yr);
}


//下面的函数是当且仅当WiFi链接成功时通过ntp协议获取时间并为rtc赋值  将此函数放在UI的loop中
void Auto_Set_Time()
{
  if((Situation_Of_Wifi()==1) && (getautoset()==0))
  {
     GetTime();
     set_time(GetTime_sec(), GetTime_min(), GetTime_hour(), GetTime_date(3), GetTime_date(2), GetTime_date(1)); 
  }
}
//输出当前的时间

void Myclock_output()
{
  int hour = rtc.getHour(1);//24小时制;'0'--12小时制
  int minute = rtc.getMinute();
  int second = rtc.getSecond();

  int year = rtc.getYear();
  int month = rtc.getMonth() + 1;
  int day  = rtc.getDay();
  int weekday = rtc.getDayofWeek();
  String APM = rtc.getAmPm();

   // 格式化时间字符串（补零）
  char timeString[12];// 多留一些空间
  snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", hour, minute, second);
  //格式化日期字符串
  char dateString[24];// 多留一些空间
  snprintf(dateString, sizeof(dateString), "%02d/%02d/%02d", year, month, day);
  //格式化输出星期
  char weekdayString[10];
    switch (weekday) {
        case 0: strcpy(weekdayString, "Sunday"); break;
        case 1: strcpy(weekdayString, "Monday"); break;
        case 2: strcpy(weekdayString, "Tuesday"); break;
        case 3: strcpy(weekdayString, "Wednesday"); break;
        case 4: strcpy(weekdayString, "Thursday"); break;
        case 5: strcpy(weekdayString, "Friday"); break;
        case 6: strcpy(weekdayString, "Saturday"); break;
        default: strcpy(weekdayString, "Unknown"); break;
    }
  WeekDay = weekdayString;            //把计算完的weekdaystring赋给全局的
  Myscreen_clear();
  Myscreen_size(2);                 //默认字体大小为‘2’-----------------------------------------------------------------------------------------------

  Myprint(0,0,dateString);
  Myprint(0,1,timeString);
  Myprint(0,2,weekdayString);
  //Myscreen_size(1);                 //AM太大了屏幕放不下，临时修改字体大小
  //Myscreen_setp(100,25);
  //Myprint_p("--"+APM);
  
  Myscreen_display();
}


int get_time(String lable)                               //lable =sc,mn,hr,dy,mt,yr
{
  if(lable == "sc")
  {
    return 0;                                     //秒数默认设置为0
  }
  else if(lable == "mn")                                        
  {
    return rtc.getMinute();                       //返回当前的分钟值
  }
  else if(lable == "hr")
  {
     return rtc.getHour(1);                        //返回当前的小时值
  }
  else if(lable == "dy")
  {
     return rtc.getDay();                         //返回当前的天值
  }
  else if(lable == "mt")
  {
    return rtc.getMonth() + 1;                    //返回当前的月份值
  }
  else if(lable == "yr")
  {
    return rtc.getYear();                         //返回当前的年值
  }
 return -1010; 
}

String getAPM()
{
                      //返回是上午还是下午
 
    return rtc.getAmPm();
  
}

String getWeekDay()
{
  return WeekDay;
}