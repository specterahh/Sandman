#ifndef MYSCREEN_H
#define MYSCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>  // 添加 Wire 库支持

extern Adafruit_SSD1306 display;  // 声明全局变量

void Myscreen_init(uint16_t address);               //屏幕初始化
void Myprint(int width,int height,String a);      //输出字符串
void Myprint(int m,int n,int a);                 //输出整形数字
void Myprint(int m,int n,char a);                //输出字符
void Myscreen_clear(void);                       //清理屏幕
void Myscreen_clear(int h);                      //h为高度，单位是像素，可以实现部分清除
void Myscreen_display(void);                     //刷新屏幕
void Myscreen_size(int a);                       //调整字体大小

void Myscreen_setp(int wid,int hei);             //可用于非格式化的位置设定(参数的单位是“像素”)
                                                 //以下输出函数用于非格式化输出
void Myprint_p(int a);
void Myprint_p(char a);
void Myprint_p(String a);

#endif