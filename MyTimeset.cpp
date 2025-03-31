#include <Arduino.h>
#include <MyClock.h>
#include <Mykey.h>
#include <GetTimeFromWeb.h>


int cursor = 5;
int *cur   = &cursor;
int AUTOmod =0;                             //用于切换是否自动获取时间的模式
String pm = "PM";
int time_group[6] = {0, 56, 22,23, 12, 2025} ;                   //设置time_set数组用于存放与显示当前的时间值
String GroupOfAuto [2]= {"AUTOon","AUTOoff"};
void settime_show() ;
void settime_display();
void keychange_group();

int getautoset()                                //返回当前AUTOmode的值，如果是0则开启自动授时，是1则关闭自动授时
{
   return AUTOmod;
}
//获取当前rtc的值，并把它存到数组里
void settime_show()                                 //lable =sc,mn,hr,dy,mt,yr
{
  time_group[0] = get_time("sc");
  time_group[1] = get_time("mn");

   time_group[2] = get_time("hr");

  time_group[2] = get_time("hr");
  time_group[3] = get_time("dy");
  time_group[4] = get_time("mt");
  time_group[5] = get_time("yr");
}

//依据当前数组的值设置rtc的值
void settime_set()
{
  set_time(time_group[0], time_group[1], time_group[2],time_group[3], time_group[4], time_group[5]);

}


//输出当前数组的值
void settime_display()
{
  Myscreen_clear();
  Myscreen_size(2);                 //默认字体大小为‘2’

  if(AUTOmod == 0)
  {
      if(Situation_Of_Wifi() != 1)  //若WiFi未连接，则显示WiFi未连接
      {
         Myscreen_size(1);
         Myprint(0,1,"wifi is not connected");
         Myscreen_display();
         Myscreen_size(2);

         delay(2000);
         Myscreen_clear();
         AUTOmod = 1;
         cursor = 5;
      }
      else
      cursor = 6;
  }

  switch (cursor)
{
  case 5:
  {
     //输出年/月/日
    Myprint(0,0,time_group[5]);
    Myscreen_setp(0,17);             //光标的绘制位置
    Myprint_p("^^^^");              //用于标识当前修改位置的光标
    Myprint(4,0,"/");
    Myprint(5,0,time_group[4]);
    Myprint(7,0,"/");
    Myprint(8,0,time_group[3]);
 
 //输出时分秒
    Myprint(0,2,time_group[2]);
    Myprint(2,2,":");
    Myprint(3,2,time_group[1]);
    Myscreen_size(1);
    Myprint(6,2,GroupOfAuto[AUTOmod]);
    Myscreen_size(2);
     break;
  }
  case 4:
  {
     //输出年/月/日
    Myprint(0,0,time_group[5]);
    Myprint(4,0,"/");
    Myprint(5,0,time_group[4]);
    Myscreen_setp(60,17);              //光标的绘制位置
    Myprint_p("^^");                  //用于标识当前修改位置的光标
    Myprint(7,0,"/");
    Myprint(8,0,time_group[3]);
 
 //输出时分秒
    Myprint(0,2,time_group[2]);
    Myprint(2,2,":");
    Myprint(3,2,time_group[1]);
    Myscreen_size(1);
    Myprint(6,2,GroupOfAuto[AUTOmod]);
    Myscreen_size(2);
   
     break;
  }
  case 3:
  {
     //输出年/月/日
    Myprint(0,0,time_group[5]);
    Myprint(4,0,"/");
    Myprint(5,0,time_group[4]);
    Myprint(7,0,"/");
    Myscreen_setp(96,17);              //光标的绘制位置
    Myprint_p("^^");                  //用于标识当前修改位置的光标
    Myprint(8,0,time_group[3]);
 
 //输出时分秒
    Myprint(0,2,time_group[2]);
    Myprint(2,2,":");
    Myprint(3,2,time_group[1]);
    Myscreen_size(1);
    Myprint(6,2,GroupOfAuto[AUTOmod]);
    Myscreen_size(2);
    
     break;
  }

  case 2:
  {
     //输出年/月/日
    Myprint(0,0,time_group[5]);
    Myprint(4,0,"/");
    Myprint(5,0,time_group[4]);
    Myprint(7,0,"/");
    Myprint(8,0,time_group[3]);
 
 //输出时分秒
    Myprint(0,2,time_group[2]);
    Myscreen_setp(0,54);              //光标的绘制位置
    Myprint_p("^^");                  //用于标识当前修改位置的光标
    Myprint(2,2,":");
    Myprint(3,2,time_group[1]);
    Myscreen_size(1);
    Myprint(6,2,GroupOfAuto[AUTOmod]);
    Myscreen_size(2);
    
     break;
  }
  
    case 1:
  {
     //输出年/月/日
    Myprint(0,0,time_group[5]);
    Myprint(4,0,"/");
    Myprint(5,0,time_group[4]);
    Myprint(7,0,"/");
    Myprint(8,0,time_group[3]);
 
 //输出时分秒
    Myprint(0,2,time_group[2]);
    Myprint(2,2,":");
    Myscreen_setp(36,54);              //光标的绘制位置
    Myprint_p("^^");                  //用于标识当前修改位置的光标
    Myprint(3,2,time_group[1]);
    Myscreen_size(1);
    Myprint(6,2,GroupOfAuto[AUTOmod]);
    Myscreen_size(2);
    
     break;
  }
    case 0:
  {
     //输出年/月/日
    Myprint(0,0,time_group[5]);
    Myprint(4,0,"/");
    Myprint(5,0,time_group[4]);
    Myprint(7,0,"/");
    Myprint(8,0,time_group[3]);
 
 //输出时分秒
    Myprint(0,2,time_group[2]);
    Myprint(2,2,":");
    Myprint(3,2,time_group[1]);
    Myscreen_size(1);
    Myscreen_setp(72,48);              //光标的绘制位置
    Myprint_p("^^^^^^");                  //用于标识当前修改位置的光标
    Myprint(6,2,GroupOfAuto[AUTOmod]);
    Myscreen_size(2);
    
     break;
  }

   case 6:
   {
      Myprint(2,1,GroupOfAuto[AUTOmod]);
   }
}
  
  Myscreen_display();
}



void keychange_group()                                    //规则设定：在时间设置页面key1短按，游标加1，长按key1退出设定界面 ,若短按key2则时间加1，长按key2时间减一                                                                
{
    if(key_judge(key2)==1)
    {
      if(*cur == 5)                                               //若此时游标的值是5则代表目前增加的是年份，年份没有设定
      {
          time_group[*cur] ++;
      }
      else if (*cur == 4)
      {
           if(time_group[*cur] == 12)                             //若此时游标值是4则代表数组中对应位置的值是月份，若当前月份为12月，则将其设置为一月
           {
              time_group[*cur] = 1;
           }
           else
          {
              time_group[*cur] ++;                                //其余情况下月份加一
          }                                             
      }
      else if(*cur == 3)
      {
        if(time_group[*cur] == 31)                             //若此时游标值是3则代表数组中对应位置的值是日期，若当前为31号，则将其设置为1号

           {                                                      //这里不用考虑是大月还是小月，库函数会自行判断并自动进位
              time_group[*cur] = 1;
           }
           else
          {
              time_group[*cur] ++;                                //其余情况下日期加一
          }                          
      }

      else if(*cur == 2)
      {
        if(time_group[*cur] == 23)                             //若此时游标值是2则代表数组中对应位置的值是小时，若当前为23时，则将其设置为0时

           {                                                     
              time_group[*cur] = 0;
           }
           else
          {
              time_group[*cur] ++;                                //其余情况下小时加一
          }                          
      }

      else if(*cur == 1)
      {
        if(time_group[*cur] == 59)                             //若此时游标值是1则代表数组中对应位置的值是分钟，若当前为59分，则将其设置为0分

           {                                                     
              time_group[*cur] = 0;
           }
           else
          {
              time_group[*cur] ++;                                //其余情况下小时加一
          }
                                    
      }                                                          //由于秒数一直在变化，我们并未给用户提供修改秒数的方法，而是将秒数直接设置为0
                                                                  //在MyClock.cpp 中的get_time函数中实现

    else if((cursor == 0) || (cursor == 6))                                         //用于设置是否开启自动授时（默认开启）
      {
          AUTOmod =  ! AUTOmod;
          cursor = 0;
      }
    }

  //当key2长按时，减少对应时间
   else if(key_judge(key2)==2)
    {
      if(*cur == 5)                                               //若此时游标的值是5则代表目前减少的是年份，年份应当大于0（假设我们的使用者在耶稣诞生之后才使用我们的产品）
      { 
        if(time_group[*cur] > 0)
        {
          time_group[*cur] --;
        }
        else;                                                       //否则没有任何动作
      }
      else if (*cur == 4)
      {
           if(time_group[*cur] == 1)                             //若此时游标值是4则代表数组中对应位置的值是月份，若当前月份为1月，则将其设置为12月
           {
              time_group[*cur] = 12;
           }
           else
          {
              time_group[*cur] --;                                //其余情况下月份减一
          }                                             
      }
      else if(*cur == 3)
      {
        if(time_group[*cur] == 1)                             //若此时游标值是3则代表数组中对应位置的值是日期，若当前为1号，则将其设置为31号

           {                                                      //这里不用考虑是大月还是小月，库函数会自行判断并自动进位
              time_group[*cur] = 31;
           }
           else
          {
              time_group[*cur] --;                                //其余情况下日期加一
          }                          
      }

      else if(*cur == 2)
      {
        if(time_group[*cur] == 0)                             //若此时游标值是2则代表数组中对应位置的值是小时，若当前为0时，则将其设置为23时

           {                                                     
              time_group[*cur] = 23;
           }
           else
          {
              time_group[*cur] --;                                //其余情况下小时减一
          }                          
      }

      else if(*cur == 1)
      {
        if(time_group[*cur] == 0)                             //若此时游标值是1则代表数组中对应位置的值是分钟，若当前为0分，则将其设置为59分

           {                                                     
              time_group[*cur] = 59;
           }
           else
          {
              time_group[*cur] ++;                                //其余情况下小时加一
          }                          
      }                                                          //由于秒数一直在变化，我们并未给用户提供修改秒数的方法，而是将秒数直接设置为0
                                                                  //在MyClock.cpp 中的get_time函数中实现
      
    }

   if(key_judge(key1) == 1)                                 //当key1短按时，游标加一
    {
      if(*cur > 0)
      {
          (*cur)--;                                              //若当前游标的值小于5，游标加一
      }
      else
      {
        *cur = 5;                                               //否则将游标设为一
      }
    }

    
}


