#include <MyClock.h>
#include <MyTimeset.h>
#include <MyScreen.h>
#include <Mykey.h>
#include <MyHtmlMenu.h>             //在自动设置的程序中，需要调动这个头文件中所声明的函数，获得当前时间的课表
#define BUZZER_CHANNEL 0    // 新增PWM通道
#define BUZZER_RESOLUTION 8 // 分辨率
#define BUZZER_PIN 27  // 假设使用ESP32的GPIO27引脚，你可以根据实际情况修改

//void blink_led();  // 函数声明

int AlarmTimeGroup[2] = {1,0};        //闹钟数组：定义一个数组用于存储当前闹钟的时间--小时--分钟（和之前设置时间的数组的时间排序不同，注意甄别）
String ON_OFF [2] = {"ON","OFF"};     //状态数组：定义一个数组来存储闹钟是否开启
//String AM_PM [2] = {"AM","PM"};       //上下午设定
String AUTO [2] = {"AUTOon","AUTOoff"};//用于储存当前闹钟是否是根据课表自动设定 
int alarm_cursor = 0;                         //游标的值从0-2
int onoroff = 0;                        //0是ON 1是OFF
int AMPMset = 0;                        //0是AM 1是PM
int Autoset = 1;                        //0是AUTO-ON 1是AUTO-OFF
//bool buzzer_state = false;  // 蜂鸣器状态标志 
//unsigned long last_blink = 0;  // 用于蜂鸣器持续响铃计时
int alarmkeep = 0;      //定义一个全局变量用于维持闹钟持续开启直到有人按下了key1或key2中的任何一个键
int doonlyonce = 100;     //用于防止Alarmjudge()函数一直在为alarmkeep 赋值为1，导致闹钟持续响铃





void setup_buzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);  // 初始设为高电平
  ledcSetup(BUZZER_CHANNEL, 5000, BUZZER_RESOLUTION); // PWM初始化
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);          // 绑定引脚
  ledcWriteTone(BUZZER_CHANNEL, 0);
}


int Alarmjudge() {                  //用于判断当前闹钟是否到时间
  
    static int last_hour = -1;  // 用于记录上次检查的小时
    static int last_minute = -1;  // 用于记录上次检查的分钟
    // 当小时或分钟发生变化时，重置 doonlyonce
    if (last_hour != get_time("hr") || last_minute != get_time("mn")) {
      doonlyonce = 100;
      last_hour = get_time("hr");
      last_minute = get_time("mn");
  }
    if(onoroff == 1)
    {
      //digitalWrite(LED_PIN, LOW);  // 确保LED关闭：我们不希望led灯只闪烁1分钟，而是希望只有用户按下任意一个按键时led灯才会熄灭
      
      return 0; 
    } // 如果闹钟关闭，直接返回0
    else if(Autoset==0)         //如果闹钟的自动模式打开了，则不使用用户设置的闹钟，而是直接依据课表执行闹钟
    {
        String mid = GetTimeTable(getWeekDay(),getAPM());
        if((mid != "No Class") && (mid != NULL))
        {

              if(get_time("mn") == 30 && 
                   get_time("hr") == 7 && 
                    (doonlyonce != get_time("mn"))) 
              {
                  alarmkeep = 1;
                  doonlyonce = get_time("mn");
                  return alarmkeep;
              }
            

            else if(getAPM() == "PM")                        //若下午第一节有课，则在下午1：00触发闹钟（提前半个小时）
            {
              if(get_time("mn") == 0 && 
                   get_time("hr") == 13 && 
                    (doonlyonce != get_time("mn"))) 
              {
                  alarmkeep = 1;
                  doonlyonce = get_time("mn");
                  //blink_led();  // 闹钟时间到时闪烁LED：由于之后还会有别的模引用这个Alarmjudge的值，我们就不在这里写开启led灯的程序了，我对blink_led()中的内容进行了小小的修改
                  return alarmkeep;
              }
            }
        } 
        
    }
    else if(get_time("mn") == AlarmTimeGroup[1] && 
       get_time("hr") == AlarmTimeGroup[0] && 
        (doonlyonce != get_time("mn"))) 
    {
      alarmkeep = 1;
      doonlyonce = get_time("mn");
       //blink_led();  // 闹钟时间到时闪烁LED：由于之后还会有别的模引用这个Alarmjudge的值，我们就不在这里写开启led灯的程序了，我对blink_led()中的内容进行了小小的修改
      return alarmkeep;
    }
    if(alarmkeep == 0)        //如果检测到当前的alarmkeep的值为0，直接返回，不给读取按键的机会！防止干扰ui正常运行
    {
      if (get_time("mn")-doonlyonce >= 1) //再响铃时间超过一分钟后再重置doonlyonce
        doonlyonce = 100;
      return 0;
    }
    else if((key_judge(key1) == 1) || (key_judge(key2) == 1) )
    {
      if (get_time("mn")-doonlyonce >= 1) //再响铃时间超过一分钟后再重置doonlyonce
        doonlyonce = 100;
      alarmkeep = 0;
      //doonlyonce = 100;
      return alarmkeep;
    }
    else if(alarmkeep == 1)       //即使闹钟响铃超过了1分钟依旧可以继续响铃
      {
        
        return alarmkeep;
      }
   //digitalWrite(LED_PIN, LOW);  // 非闹钟时间保持LED关闭：我们不希望led灯只闪烁1分钟，而是希望只有用户按下任意一个按键时led灯才会熄灭
   
   return -1;
}






void Alarm_display()              //用于显示调整时间的界面即显示闹钟数组和状态数组的值，并标识出游标所在的位置
{
  Myscreen_clear();
  Myscreen_size(2); 
  if (Autoset == 0)                //若当前开启了自动闹钟设置，则不允许用户自己设置别的闹钟，而是完全依据课表设置的闹钟来
  {
    alarm_cursor = 5; 
  }

  switch (alarm_cursor)
{
  case 0:                           //修改小时的页面
  {
    //输出时、分
    Myprint(0,0,AlarmTimeGroup[0]);
    
    Myprint(0,1,"^^");              //用于标识当前修改位置的光标
    Myprint(2,0,":");
    Myprint(3,0,AlarmTimeGroup[1]);
    //Myprint(6,0,AM_PM [AMPMset]);
    Myprint(0,2,ON_OFF[onoroff]);
    Myprint(3,2,AUTO [Autoset]);
    
    break;
  }
  case 1:                           //修改分钟的页面
  {
     //输出时、分
    Myprint(0,0,AlarmTimeGroup[0]);
    Myprint(3,1,"^^");              //用于标识当前修改位置的光标
    Myprint(2,0,":");
    Myprint(3,0,AlarmTimeGroup[1]);
    //Myprint(6,0,AM_PM [AMPMset]);
    Myprint(0,2,ON_OFF[onoroff]);
    Myprint(3,2,AUTO [Autoset]);
    break;
   
  }

 
  
  case 2:
  {
    //输出时、分
    Myprint(0,0,AlarmTimeGroup[0]);
   
    Myprint(2,0,":");
    Myprint(3,0,AlarmTimeGroup[1]);
    Myprint(0,2,ON_OFF[onoroff]);
    //Myprint(6,0,AM_PM [AMPMset]);
    Myprint(0,3,"^^");              //用于标识当前修改位置的光标
    Myprint(3,2,AUTO [Autoset]);
    break;
  }

  case 3:
  {
    //输出时、分
    Myprint(0,0,AlarmTimeGroup[0]);
   
    Myprint(2,0,":");
    Myprint(3,0,AlarmTimeGroup[1]);
    Myprint(0,2,ON_OFF[onoroff]);
   //Myprint(6,0,AM_PM [AMPMset]);
    Myprint(3,3,"^^^^^^^");              //用于标识当前修改位置的光标
    Myprint(3,2,AUTO [Autoset]);
    break;
  }
  case 5:
  {
    Myprint(2,1,AUTO [Autoset]);          //若当前开启了自动闹钟设置，则不允许用户自己设置别的闹钟，而是完全依据课表设置的闹钟来
    
  }
}

  Myscreen_display();
}







void alarmkeychange_group()                                    //规则设定：在时间设置页面key1短按，游标加1，长按key1退出设定界面 ,若短按key2则时间加1                                                                
{
    if(key_judge(key2)==1)
    {
       if(alarm_cursor == 0)
      {
        if(AlarmTimeGroup[alarm_cursor] == 23)                             //若此时游标值是0则代表数组中对应位置的值是小时，若当前为23时，则将其设置为0时

           {                                                     
              AlarmTimeGroup[alarm_cursor] = 0;
           }
           else
          {
              AlarmTimeGroup[alarm_cursor] ++;     
                         //其余情况下小时加一
          }                          
      }

      else if(alarm_cursor == 1)
      {
        if(AlarmTimeGroup[alarm_cursor] == 59)                             //若此时游标值是1则代表数组中对应位置的值是分钟，若当前为59分，则将其设置为0分

           {                                                     
              AlarmTimeGroup[alarm_cursor] = 0;
           }
           else
          {
              AlarmTimeGroup[alarm_cursor] ++;                                //其余情况下分钟加一
          }                         
      }

      else if(alarm_cursor == 2)                                            //若此时游标的值是2则对应的是闹钟状态的数组
      {
          onoroff = ! onoroff;
      }

      

      else if((alarm_cursor == 3) || (alarm_cursor == 5))                                            //若此时游标的值是2则对应的是闹钟状态的数组
      {
          Autoset = ! Autoset;
          alarm_cursor = 0;
      }                                                                                                        
    }

  if(key_judge(key1) == 1)                                 //当key1短按时，游标加一
    {
      if(alarm_cursor < 3 )
      {
          alarm_cursor ++;                                              //若当前游标的值小于3，游标加一
      }
      else
      {
        alarm_cursor = 0;                                               //否则将游标设为一
      }
    }


}

int freq[] = {131,147,165,175,196,220,247,262,294,330,349,370,392,411,440,494,523,587,659,698,784,880,988};
int qianzou[] = {5,7,12,7,3,7,12,7,5,7,12,7,0,12,6,7}; // 前奏部分
int melody[] = {  // 合并主旋律
  5,7,12,7,3,7,12,7,5,7,12,7,0,12,6,7, // 重复前奏
  -1 // 结束标记
};



//unsigned long previousMillis = 0; // 记录上一次播放音符的时间
//int currentNote = 0; // 当前播放的音符索引

// 修改播放函数
void play_music() {
  static unsigned long previousMillis = 0;
  static int currentNote = 0;
  
  if(melody[currentNote] == -1) { // 循环检测
    currentNote = 0;
  }

  if(millis() - previousMillis >= 500) {
    int note = melody[currentNote];
    if(note >= 0) {
      ledcWriteTone(BUZZER_CHANNEL, freq[note]);
    } else {
      ledcWriteTone(BUZZER_CHANNEL, 0);
    }
    previousMillis = millis();
    currentNote++;
  }
}


void blink_buzzer(int judge) {
  if(judge == 0) {
    ledcWriteTone(BUZZER_CHANNEL, 0); // 直接静音
    ledcDetachPin(BUZZER_PIN);     // 断开PWM连接
    digitalWrite(BUZZER_PIN, HIGH);    // 双重保障
    return;
  }
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);  // 重新连接PWM

  for(int i=0; i<255; i+=10) {
    ledcWrite(BUZZER_CHANNEL, i);
    delay(10);
  }
  play_music();
}




