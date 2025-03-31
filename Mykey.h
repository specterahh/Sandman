#ifndef _MYKEY_H
#define _MYKEY_H
#include<Arduino.h>
#define key1  2                                       //第一个按键链接的是5号引脚
#define key2  4                                      //第二个按键链接的是18号引脚
void setkey_input(int pin);
void setkey_input_down(int pin);
void setkey_input_up(int pin);
int key_judge(int pin);
#endif
