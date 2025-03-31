#ifndef __MYTIMESET_H
#define __MYTIMESET_H

void settime_show() ;                               //读取当前的时间并把它设置到数组里面
void settime_display();                             //输出当前数组的值
void keychange_group();
void settime_set();                                 //依据当前数组中的值为rtc赋时
int getautoset();                                   //返回当前AUTO的值   
#endif