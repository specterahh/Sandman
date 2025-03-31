#include <Arduino.h>
#include <MyMenu.h>
#include <MyAlarmset.h>
#include <MyHtmlMenu.h>
#include <ESP-now-connect.h>
#include <MytfPlayer.h>
#include <Ledmatrix.h>
unsigned long lastUpdateTime = 0;

/**
 * 库自带顶层函数，用于初始化
 */
void setup() {
    
    Serial.begin(115200);    // 添加串口调试
    delay(1000); // 等待串口初始化完成
    ledMatrix_init();
    startpage();
    MyHtmlMenuSetup();                                              //配置web服务器与WiFi                     
    
    ESP_INIT();

    menu_setup();                                                   //配置主界面和目录界面


    
  
}

void loop() {
    int alarm= Alarmjudge();
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= 1000) {
      lastUpdateTime = currentTime;
      send(alarm);
    }  
    Auto_Set_Time(); //当WiFi链接时通过网络为rtc校准时间
    int alarm_status = alarm; // 判断闹钟状态
    /*
        if(Alarmjudge() == 0){  // 当闹钟停止时重置播放
      ledcWriteTone(BUZZER_CHANNEL, 0);
      ledcDetachPin(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(10); // 降低GPIO刷新频率
    }   
    */
                          
    //blink_buzzer(Alarmjudge() );                 //用于判断当前闹钟是否响铃，每次循环都要判断
    mp3ONoroff( alarm);
    Serial.println(alarm);
    menu_run();                               //判断完闹钟响铃时的按键逻辑后再判断页面的按键逻辑
}



