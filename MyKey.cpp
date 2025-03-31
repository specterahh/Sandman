#include <Arduino.h>

int onlyonce = 0;
    
     void setkey_input(int pin)
    {
        pinMode(pin,INPUT);
    }

    void setkey_input_down(int pin)            //定义引脚为下拉输入(长期维持在低电平，按键导通变为高电平)
    {
        pinMode(pin,INPUT_PULLDOWN);
    }

    void setkey_input_up(int pin)              //定义引脚为上拉输入（长期维持在高电平，按键导通变成低电平）
    {
        pinMode(pin,INPUT_PULLUP);
    }


//ATTENTION:下面的按键行为判断函数是针对下拉输入做到，无法适配上拉输入和浮空输入

    int key_judge(int pin)                                 //定义一个函数用于检测按键的行为
    {
        int key_logic = 0;                          //定义一个变量用于记录按键的行为
        int counter = 0;                            //定义一个变量用于判断按键长按
        //按键消抖
        if(digitalRead(pin) == 1)           //当key的引脚为高电平时
        {
            //延迟15毫秒
            delay(15);
            while(digitalRead(pin) == 1)     //在按键松开前不要执行程序
            {
                delay(10);                      //0.01s循环一次即如果按键长按1.5秒就是循环150次
                if (counter>=75)
                {
                    onlyonce = 1;
                    return 2;                       //如果有人长按2秒则判定为长按按键，返回2
                }
                else
                    counter ++;    
            }
            if(onlyonce == 1)
            {
                delay(200);
                onlyonce = 0;
                return 0;
            }
            return 1;                               //否则算短按返回1
        }
        return 0;
    }



