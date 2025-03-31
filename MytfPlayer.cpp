/*
#include "Arduino.h"                                //调用库文件
#include "SoftwareSerial.h"                         //调用库文件
#include "DFRobotDFPlayerMini.h"                    //调用库文件
 
int volume1 = 10;                                  //控制mp3音量，范围从0到30
SoftwareSerial mySoftwareSerial(17, 16);              //连接模块的TX和RX引脚
DFRobotDFPlayerMini myDFPlayer;                     //实例化对象


void mp3_init()
{

   // Serial.begin(9600);                               //设置串口波特率为9600
  delay(20000);
  //Serial.println("*************************************************");
  mySoftwareSerial.begin(9600);                     //初始化软串口
 
  if (!myDFPlayer.begin(mySoftwareSerial)) {        //使用serial软件与mp3通讯。
    while (millis() <= 1000) {
      //Serial.println("设备即将上线。");
     // Serial.println("***************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
    }
  }
  myDFPlayer.volume(volume1);                        //设置音量值。从0到30
  //myDFPlayer.loop(1);                              //循环播放第一个mp3
  
  //myDFPlayer.next();                               //播放下一个mp3
  myDFPlayer.play(1);                                //播放第一个mp3
  myDFPlayer.enableLoopAll();                        //循环播放列表
}




void mp3_stop()
{
    myDFPlayer.pause();                              //暂停mp3,关闭音乐
}


void volume(int a)                                  //a : 0~30
{
    volume1 = a;
}


void mp3onoff(int a)
{
    if(a == 0)
    {
        mp3_stop();
    }
    else if(a == 1)
    {
        myDFPlayer.start();
    }
}

*/

#include "Arduino.h"                                //调用库文件
#include "driver/gpio.h"
#define start_pin 32
#define stop_pin 33
int duonlyonce_mp3 = 0;
void disable_gpio(gpio_num_t gpio_num) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpio_num), // 选择要禁用的引脚
        .mode = GPIO_MODE_DISABLE,         // 禁用该引脚
        .pull_up_en = GPIO_PULLUP_DISABLE, // 禁用上拉电阻
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // 禁用下拉电阻
        .intr_type = GPIO_INTR_DISABLE     // 禁用中断
    };
    gpio_config(&io_conf); // 配置引脚
}


void mp3ONoroff(int a)
{
    disable_gpio(GPIO_NUM_33);
    disable_gpio(GPIO_NUM_32);
  /**
   * 暂停、开启音乐(主要用于暂停音乐)
   */
  if (a == 0 && duonlyonce_mp3 == 1)
  {
      pinMode(stop_pin,OUTPUT);
      digitalWrite(stop_pin, LOW);
      delay(300);
      disable_gpio(GPIO_NUM_33); // 禁用----------------------------------------------------修改引脚时注意修改这里的值
      duonlyonce_mp3 = 0;
  }
  /**
   * 播放音乐
   */
  else if(a == 1 && duonlyonce_mp3 == 0){
      pinMode(32,OUTPUT);
      digitalWrite(32, LOW);
      delay(300);
      disable_gpio(GPIO_NUM_32); // 禁用----------------------------------------------------修改引脚时注意修改这里的值
      duonlyonce_mp3 = 1;
  }
}



