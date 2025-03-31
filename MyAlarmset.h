#ifndef __MYALARMSET_H
#define __MYALARMSET_H
#define BUZZER_CHANNEL 0    // 必须全局可见
#define BUZZER_PIN 27  // 假设使用ESP32的GPIO27引脚，你可以根据实际情况修改
void Alarm_display();
void alarmkeychange_group() ;
int Alarmjudge();
void setup_buzzer();
void blink_buzzer(int judge);
void play_music();
#endif