#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>                  
#include <ctime>                      // 包含标准时间库，用于从秒数到年月日的换算
#include <MyScreen.h>
#include <Preferences.h>              //用于对数据进行非易失性存储
// 新增AP配置全局变量
//const String ap_ssid = "ESP32_AP";      // 默认AP名称
//const String ap_password = "12345678";  // 默认AP密码（至少8位）

// NTP 客户端
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 10000); // 设置更新间隔为 1分钟

Preferences preferences;  // 创建Preferences对象，用于进行非易失性存储

void SaveWifi(const char * ssid,const char* password);//用于记忆链接成功的WiFi的ssid和密码
void SetWifi();//用于读取上一次链接成功的WiFi信息来尝试链接WiFi
void SetWifi_e();
// 将 Unix 时间戳转换为年、月、日
struct tm* convertEpochToDateTime(unsigned long epoch) {
  time_t rawTime = epoch;
  struct tm *timeInfo;
  timeInfo = gmtime(&rawTime); // 使用 GMT 时间
  return timeInfo;
}






void wifi_ntpinit(const char * ssid,const char *password)
{
  // 新增AP状态保持
  if(WiFi.getMode() != WIFI_AP_STA){
    WiFi.mode(WIFI_AP_STA); // 确保AP模式立即生效
  }

  Myscreen_init(0x3c);          //进行屏幕初始化
  Myscreen_clear(); 
  Myscreen_size(1);




  if((ssid != "a") && (password != "a"))
  {
    Myprint(0,1,"connecting...");
    int counter = 0;
      // 连接到 WiFi 网络
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      counter ++;
      Myscreen_setp(counter*3,34);
      Myprint_p("/") ;
      Myscreen_display();
      if(counter >= 40) //如果20秒后依然没有链接上WiFi，则跳过链接WiFi的步骤
      {


          Myscreen_clear();
          Myprint(0,0,"WiFi not connected.");
          Myscreen_display();
          delay(6000); 
          Myscreen_clear();
          break ;                        //
      }


      if (WiFi.status() == WL_CONNECTED)  //如果WiFi当前的状态是链接成功则将这个成功的WiFi名称和WiFi密码存储到
                                          //非易失性存储中，下一次如果没有配网自动使用保存的ssid和password进行联网尝试
      {
          SaveWifi(ssid,password);
      }

    }
  }
  else
  {
         SetWifi(); //用于读取上一次链接成功的WiFi信息来尝试链接WiFi(程序刚刚启动时执行的部分)
  }
  


  // 初始化 NTP 客户端
  timeClient.begin();
  timeClient.setTimeOffset(28800);        //设置时区为东8区
}





int Situation_Of_Wifi()
{
    if(WiFi.status() == WL_CONNECTED)
    {
      return true;                             //若当前WiFi链接成功则返回1
    }

    return false;                               //其他情况返回0
}




void GetTime()
{

  timeClient.update();                //从网络中获取时间(要确保这个函数包含在loop中)
}





int GetTime_hour()
{
  return (timeClient.getHours());     //获取小时，并将其返回
}

int GetTime_min()
{
  return (timeClient.getMinutes());   //获取当前的分钟，并将其返回
}

int GetTime_sec()
{
  return (timeClient.getSeconds());   //获取当前的秒数，并将其返回
}




int GetTime_date(int a)               //参数说明：a=1返回年份，a=2返回月份，a=3返回日期
{
    // 获取 Unix 时间戳
  unsigned long epochTime = timeClient.getEpochTime();

  // 转换为年、月、日
  struct tm *timeInfo = convertEpochToDateTime(epochTime);

  // 提取年、月、日
  int year = timeInfo->tm_year + 1900; // tm_year 是从 1900 开始的
  int month = timeInfo->tm_mon + 1;    // tm_mon 是从 0 开始的
  int day = timeInfo->tm_mday;         // tm_mday 是从 1 开始的

  switch (a)
  {
    case 1:
    {
      return year;
    }

    case 2:
    {
      return month;
    }

    case 3:
    {
      return day;
    }
  }
  return -1;
}



// 将数据保存到EEPROM(这样即使板子断电重启里面存储的东西也不会消失)的函数
void SaveWifi(const char * ssid,const char* password) {
    preferences.begin("ssid-password", false);  // 打开一个名为"ssid-password"的命名空间，false表示以读写模式打开
    
    preferences.putString("ssid", ssid);  // 将数据存储到名为"ssid"的键中
    preferences.putString("password", password);  // 将数据存储到名为"password"的键中

    preferences.end();  // 关闭Preferences
}


void SetWifi() {
  
  
  WiFi.mode(WIFI_AP_STA);  // 确保AP模式立即生效

  // 从非易失性存储读取凭证
  preferences.begin("ssid-password", true);
  String savedSsid = preferences.getString("ssid");
  String savedPassword = preferences.getString("password");
  
  preferences.end();


  preferences.begin("ap_config",true);

  String savedAP_ssid = preferences.getString("ssid");
  String savedAP_password = preferences.getString("password");
  preferences.end();

  WiFi.softAP(savedAP_ssid.c_str(),savedAP_password.c_str());  // 确保AP生效(初始化AP)

  if(savedSsid.isEmpty() || savedPassword.isEmpty()) {
      Myscreen_clear();
      Myprint(0, 0, "No saved WiFi");
      Myscreen_display();
      delay(2000);
      return;/*  */
  }




  Myscreen_size(1);
  Myprint(0, 1, "starting...");
  Myscreen_display();

  WiFi.begin(savedSsid.c_str(), savedPassword.c_str());
  WiFi.setAutoConnect(true);
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED && counter < 40) {
      delay(500);
      counter++;
      
      Myscreen_setp(counter*3, 34);
      Myprint_p("/");
      Myscreen_clear(15);             //清除0-15行的内容
      Myscreen_setp(0, 0);
      Myprint_p("Trying: " + String(counter*0.5) + "s");
      Myscreen_display();
  }

  if(WiFi.status() != WL_CONNECTED) {
      Myscreen_clear();
      Myprint(0, 0, "Connect failed");
      Myprint(0, 16, "SSID:" + savedSsid);
      Myscreen_display();
      delay(6000);
      Myscreen_clear();
  }


    // 修改2：先启动AP再连接STA

  
  // 修改3：移除冗余的softAP调用
  // 原代码的这行会覆盖AP配置，改为：
 // if(!WiFi.softAPgetStationNum()) {
   //   WiFi.softAP(ap_ssid.c_str(), ap_password.c_str()); // 增强信号
  //}
}

void SetWifi_e()
{
   // 从非易失性存储读取凭证
preferences.begin("ssid-password", true);
String savedSsid = preferences.getString("ssid");
String savedPassword = preferences.getString("password");

preferences.end();
WiFi.begin(savedSsid.c_str(), savedPassword.c_str());
WiFi.setAutoConnect(true);

}

String Getssid()                    //当WiFi链接成功时返回WiFi的ssid
                                  //这个函数并不包含判断WiFi是否连接成功的功能
{
    preferences.begin("ssid-password", false);  // 打开命名空间
    String mid = preferences.getString("ssid"); //从其中导出ssid
    preferences.end();  // 关闭Preferences
    return mid;
}