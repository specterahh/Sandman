#include <Arduino.h>                                  //没这个头文件啥都不好使
#include <MyScreen.h>                                 //用于初始化屏幕并从屏幕输出特定的信息
#include <Mykey.h>                                    //用于初始化按键所对的端口与判断按键是否长按
#include <MyClock.h>                                  //用于运行时钟
#include <MyTimeset.h>                                //设置时间的界面
#include <MyAlarmset.h>
#include <MytfPlayer.h>
#include <Ledmatrix.h>
#include <MyClock.h>
#include <Upd.h>
#define BUZZER_PIN 27                                     //定义蜂鸣器的引脚
void display_menu(int a);                             //显示菜单函数的声明
int changepage();                                     //用于菜单界面的“翻页”
int logic1 = 0;                                       //用于储存按键1的按动情况
int logic2 = 0;                                       //用于储存按键2的按动情况
int set1 = 0;                                         //用于暂存页面状态
int pagenumber [3] = {1,2,3};                         //用于存储页面序号的数组（有更好的方法，但懒得改了，屎山代码大概就是这么得到的吧）
int i = 0;                                            //上面数组的游标
int set2 = 0;                                         //用于暂存每个页面的状态
int counter_M = 0;
void TurnToTimeSetPage(int where) ;                   //用于将画面跳转到时间设置界面

void menu_run(void)                                   //这个函数最后要丢到loop（）里面跑，是整个ui运行的核心
{
    //upanddown();
    logic1 = key_judge(key1);  
    logic2 = key_judge(key2);
    int mid = 0;
    //blink_buzzer(Alarmjudge() );                 //用于判断当前闹钟是否响铃，每次循环都要判断
   //mp3ONoroff( Alarmjudge());
  
  if(logic2 == 1)
  {
    if(counter_M <=3)
    counter_M ++;
    else
    counter_M = 0;
  }
    if(counter_M == 0)
    {
      shoutime(get_time("hr"),get_time("mn"));
    }
    else if(counter_M == 1)
    {
      startpage();
      
    }
    else if(counter_M == 2)
    {
      creeper();
    }
    else if(counter_M == 3)
    {
      stive1();
    }
    else if(counter_M == 4)
    {
      colors_spirit1();
    }

    if(logic1 == 2  )
     {
        
         set1 = ! set1;
     }
 
    switch(set1)                                        //在时间页面和主页面之间切换
        {
            case 1:
            {
              mid = changepage();
              display_menu(pagenumber [mid]);          
              TurnToTimeSetPage(pagenumber [mid]);
              break;               
            }
            case 0:
            {
              Myclock_output();
             
              break;
            }
        } 
}


void menu_setup()
{
     //初始化OLED屏幕
  Myscreen_init(0x3c); 
  //初始化按键所链接的引脚
  setkey_input_down(key1);                   
  setkey_input_down(key2);
  upanddinit();//初始化电机
   //初始化时钟
  MyClock_nint();
  //初始化LED
  setup_buzzer();  
     
}




void display_menu(int a)
{

  Myscreen_size(2);                                             //使用2号字体
  Myscreen_clear();                                             //清理屏幕
  switch(a)                                                     //switch用于翻页
  {
    case 1:                                                     //第一页
    {
      Myprint(0,0,"> Time-set");
      Myprint(0,1,"Alm-set");
      Myprint(0,2,"Rec-set");
    
      break;
    }

    case 2:                                                     //第二页
    {
      Myprint(0,0,"Time-set");
      Myprint(0,1,"> Alm-set");
      Myprint(0,2,"Rec-set");
    
      break;
    }

    case 3:                                                     //第三页
     {
      Myprint(0,0,"Time-set");
      Myprint(0,1,"Alm-set");
      Myprint(0,2,"> Rec-set");
    
        
    }
  }
  Myscreen_display();                                           //输出显示内容

}

int changepage()                                                //古希腊掌管翻页的神
{
  if (logic1 == 1)                                              //短按一号按键
  {
      
      if(set2<2)
      {
        set2 ++;
      }
      else if(set2 == 2)
      {
        set2 = 0;
      }
  }
    
  return set2;
}

/*下面这个函数是一个用于从菜单界面跳转到时间设置界面的函数，这个函数需要传入一个参数：参数用于表明当前
所选中的位置；与判断按键2是否短按的程序同时放到if中去做判断，如果同时成立，则会进入一
个while（）循环，这个循环中的条件是“只要按键1没有被长按就一直死循环”，在循环体的里面放的是MySettime.h中声明的
函数，用于维持时间设置页面的运行*/
void TurnToTimeSetPage(int where)                                          
{




  if(key_judge(key2) == 1)
  {
    switch(where)
    {
      case 1:
      {
        settime_show() ;                                          //将rtc中读取时钟信息并把它存到数组里
         while (key_judge(key1) != 2)                              //只要1号按键不长按就一直循环下去
          {
           settime_display();                                      //输出当前储存时间数组的值
           keychange_group();                                          
          }
           settime_set();                                             //从循环出来以后为rtc赋值
      }

      case 2:
      {
         while (key_judge(key1) != 2)                              //只要1号按键不长按就一直循环下去
          {
            Alarm_display();
            alarmkeychange_group() ;                    
          }
        break;
      }
    }
    
  }


}