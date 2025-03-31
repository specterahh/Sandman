#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>                   //1-3均为OLED屏幕的驱动；前两个需要在PIO的libraries中添加
#include <Arduino.h>                

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// 定义全局变量
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//直接可以调用数组的值来实现规范化的字符的位移
int width [] = {0,12,24,36,48,60,72,84,96,108};   
int height[] = {0,17,34,51};

//OLED屏幕初始化

void Myscreen_init(uint16_t address)    //SSD1306芯片OLED屏幕的i2c地址多半是0x3c或ox3d(似乎以0x3c为主)
{
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, address)) {
        Serial.println("SSD1306 allocation failed");
        while (1);  // 地址不对就进入无限循环
    }

    display.setTextColor(SSD1306_WHITE);
}


//此函数用于输出字符串，需要三个参数，前两个参数用于设置开始输出时的位置
//参数格式：Myprint(width, height,想要输出的内容)

void Myprint(int m,int n,String a)

{
    
    display.setCursor(width[m], height[n]);
    display.println(a);
    //display.display();
}

//此函数用于输出数字
void Myprint(int m,int n,int a)

{
    display.setCursor(width[m], height[n]);
    display.print(a);
    //display.display();
}

//此函数用于输出字符
void Myprint(int m,int n,char a)

{
    display.setCursor(width[m], height[n]);
    display.print(a);
    //display.display();
}

// 清除屏幕内容
void Myscreen_clear(void)
{
    display.clearDisplay();  
}

void Myscreen_clear(int h)                      //h为高度，单位是像素，可以实现部分清除
{
    display.clearDisplay(h);  
}

//刷新屏幕显示
void Myscreen_display(void)
{
    display.display();
}

//设置字体大小
void Myscreen_size(int a)
{
    display.setTextSize(a);
}


//可用于非格式化的位置设定
void Myscreen_setp(int wid,int hei)

{
    display.setCursor(wid, hei);

}

//与非格式化的位置设定相配合的输出
void Myprint_p(int a)
{
    display.print(a);
}

void Myprint_p(char a)
{
    display.print(a);
}

void Myprint_p(String a)
{
    display.print(a);
}