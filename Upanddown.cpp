#include <Arduino.h>
#include <Mykey.h>
 // 定义控制引脚
#define AIN1 27  // AIN1 引脚连接到 GPIO 26
#define AIN2 14  // AIN2 引脚连接到 GPIO 27
#define PWMA 12  // PWMA 引脚连接到 GPIO 25
#define STBY 26  // STBY 引脚连接到 GPIO 23

int situation = 1;//0代表闪光灯头当前在最低点，1代表在最高点
int judgebykey = 0;
int lightonoff = 0;
int counter_ud = 0;
int onlyOnce = 0;
 
 
void upanddinit()
{
    // 设置引脚模式
 pinMode(AIN1, OUTPUT);
 pinMode(AIN2, OUTPUT);
 pinMode(PWMA, OUTPUT);
 pinMode(STBY, OUTPUT);
 pinMode(25, OUTPUT);
 setkey_input_down(key1) ;
 setkey_input_down(key2); 

 // 使能模块
 digitalWrite(STBY, HIGH);
 //digitalWrite(25, HIGH);
//digitalWrite(AIN1, LOW);//上升
//digitalWrite(AIN2, HIGH);
//analogWrite(PWMA, 255);
}

void upanddown()

{
    if(key_judge(key2)==2&& onlyOnce ==0 )
  {
    judgebykey = ! judgebykey;
    onlyOnce = 1;
  }
  if(situation == 1 && judgebykey)
  {
    digitalWrite(25, LOW);
    digitalWrite(AIN1, HIGH);//下降
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, 255);
    delay(10);
    counter_ud++;
    if(counter_ud >= 4900)  //上升了29秒后，将状态定义为开启
    {
      situation = 0;
      counter_ud = 0;
      onlyOnce = 0;
      digitalWrite(AIN1, LOW);//下降
      digitalWrite(AIN2, LOW);
      analogWrite(PWMA, 255);
      delay(100);
      judgebykey = 0;
    }
    
  }
  else  if(situation == 0 && judgebykey)
  {
    digitalWrite(25, LOW);
    digitalWrite(AIN1, LOW);//上升
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, 255);
    delay(10);
    counter_ud++;
    if(counter_ud >= 4900)  //上升了29秒后，将状态定义为开启
    {
      situation = 1;
      counter_ud = 0;
      onlyOnce = 0;
      digitalWrite(AIN1, LOW);//下降
      digitalWrite(AIN2, LOW);
      analogWrite(PWMA, 255);
      delay(100);
      judgebykey = 0;
    }
    if(situation == 1 && judgebykey == 0)
    {
        digitalWrite(25, HIGH); 

    }
    
  }
}

