#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <GetTimeFromWeb.h>
#include <Preferences.h>              //用于对数据进行非易失性存储

Preferences ap_prefs;               //创建一个存储空间用于存储ESP32热点的名称和密码
const char* default_ap_ssid = "ESP32_AP";
const char* default_ap_password = "12345678";

// 统一成功提示页面 (放在全局区域或函数内)
const char* SUCCESS_HTML = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <style>
            body {
                font-family: Arial, sans-serif;
                text-align: center;
                padding: 50px;
                background: #f0f0f0;
            }
            .success-box {
                background: white;
                padding: 30px;
                border-radius: 10px;
                box-shadow: 0 0 10px rgba(0,0,0,0.1);
                display: inline-block;
            }
            .checkmark {
                color: #4CAF50;
                font-size: 50px;
                margin-bottom: 20px;
            }
            .home-link {
                display: inline-block;
                margin-top: 20px;
                padding: 10px 20px;
                background: #4CAF50;
                color: white;
                text-decoration: none;
                border-radius: 5px;
            }
        </style>
    </head>
    <body>
        <div class="success-box">
            <div class="checkmark">✓</div>
            <h1>Configuration Saved Successfully!</h1>
            <p>Your settings have been updated.</p>
            <a href="/" class="home-link">Back to Home</a>
        </div>
    </body>
    </html>
    )rawliteral";
    

AsyncWebServer server(80);  // 创建一个异步 Web 服务器，监听端口 80

Preferences preferences1;  // 创建Preferences对象，用于进行非易失性存储

//创建一个数组用于存储课表中每一个量在/save目录中的名称
const char * timetablegroup[14] = {"mon1","tue1","wed1","thu1","fri1","sat1","sun1","mon5","tue5","wed5","thu5","fri5","sat5","sun5"};

void MyHtmlMenuSetup()
{
      // 清理现有连接
      WiFi.disconnect(true);  // 彻底关闭STA
      delay(500);
      WiFi.mode(WIFI_AP_STA); 

      delay(500);
     
    

    // 设置 Web 服务器的路由
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // 发送 HTML 表单页面
    String html1 = R"(
        <!DOCTYPE html>
        <html lang='en'>
        <head>
            <meta charset='UTF-8'>
            <title>设备设置</title>
            <style>
                .section {
                    margin: 20px;
                    padding: 15px;
                    border: 1px solid #ccc;
                    border-radius: 8px;
                }
                h1 { color: #444; }
                input[type='submit'] {
                    padding: 8px 20px;
                    background: #4CAF50;
                    color: white;
                    border: none;
                    border-radius: 4px;
                }
            </style>
        </head>
        <body>
            <div class='section'>
                <h1>📶 AP热点设置</h1>
                <form action='/ap_setup' method='get'>
                    <input type='submit' value='修改热点密码'>
                </form>
            </div>
        
            <div class='section'>
                <h1>🌐 WIFI连接设置</h1>
                <form action='/setup' method='get'>
                    <input type='submit' value='设置外部WiFi'>
                </form>
            </div>
        
            <div class='section'>
                <h1>📅 课表设置</h1>
                <form action='/timetabe' method='get'>
                    <input type='submit' value='设置课程表'>
                </form>
            </div>
        </body>
        </html>
        )";
    request->send(200, "text/html", html1);
  });



    server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request) {
    // 发送 配置WiFi的表单页面
    if(Situation_Of_Wifi()==1)
    {String html21 =  R"(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <title>设置WiFi</title>
</head>
<body>
    <h1>设置WiFi</h1>
    <h2>当前WiFi:
    
)";



    String html22 =Getssid();
    String html23 = R"(已连接</h2>
    <form action='/save' method='post'>
        <label for='ssid'>WiFi SSID:</label><br>
        <input type='text' id='ssid' name='ssid'><br><br>
        <label for='password'>WiFi Password:</label><br>
        <input type='text' id='password' name='password'><br><br>
        <input type='submit' value='保存'>
    </form>
</body>
</html>)";
    request->send(200, "text/html", html21+html22+html23);
    }           //当WiFi链接成功时，再WiFi设置页面要有提示
    else
    {
        String html21 =  R"(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <title>设置WiFi</title>
</head>
<body>
    <h1>设置WiFi</h1>
    <h2>当前WiFi未连接，已保存的WiFi:
    
)";

    String html22 = Getssid();
    String html23 = R"(</h2>
    <form action='/save' method='post'>
        <label for='ssid'>WiFi SSID:</label><br>
        <input type='text' id='ssid' name='ssid'><br><br>
        <label for='password'>WiFi Password:</label><br>
        <input type='text' id='password' name='password'><br><br>
        <input type='submit' value='Save'>
    </form>
</body>
</html>)";
    request->send(200, "text/html", html21+html22+html23);;
    }
  });

server.on("/timetabe", HTTP_GET, [](AsyncWebServerRequest *request) {
     preferences1.begin("timetable", false);  // 打开一个名为"timetable"的命名空间，false表示以读写模式打开
    // 发送 配置课表的表单页面
    
    String html31 = R"(    <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name = "viewpoint" contents = "width=device-width",initial-scale=1.0">
    <title>课程表单</title>
    <style>
      input[type="text"]{
        width: 60px;
        height:15px;
      }
    </style>

</head>
<body>
    <h1>课程表单</h1>
    <form action="/submit_grades" method="post">
        <table border="1">
            <tr>
                <th>时间/节次</th>
                <th>星期一</th>
                <th>星期二</th>
               
                <th>星期三</th>
                
                <th>星期四</th>
                
                <th>星期五</th>
                
                <th>星期六</th>
                 
                <th>星期日</th>
                
            </tr>
            <tr>
                <td>第1节</td>
                <!-- 星期一第1节 -->
                <th><input type="text" name="mon1"></th>
                 <!-- 星期二第1节 -->
                <th><input type="text" name="tue1"></th>
                <!-- 星期三第1节 -->
                <th><input type="text" name="wed1"></th>
                <!-- 星期四第1节 -->
                <th><input type="text" name="thu1"></th>
                <!-- 星期五第1节 -->
                <th><input type="text" name="fri1"></th>
               <!-- 星期六第1节 -->
                <th><input type="text" name="sat1"></th>
                <!-- 星期日第1节 -->
                <th><input type="text" name="sun1"></th>
            </tr>
            


               
               <tr>
                <td>第5节</td>
                <!-- 星期一第5节 -->
                <th><input type="text" name="mon5"></th>
                 <!-- 星期二第5节 -->
                <th><input type="text" name="tue5"></th>
                <!-- 星期三第5节 -->
                <th><input type="text" name="wed5"></th>
                <!-- 星期四第5节 -->
                <th><input type="text" name="thu5"></th>
                <!-- 星期五第5节 -->
                <th><input type="text" name="fri5"></th>
               <!-- 星期六第5节 -->
                <th><input type="text" name="sat5"></th>
                <!-- 星期日第5节 -->
                <th><input type="text" name="sun5"></th>
                <td><input type='submit' value='保存'></td>
            </tr>

        </form>

    
    <form action="/submit_grades" method="post">
        <table border="1">
            <tr>
                <th>已储存的表单</th>
                <th>星期一</th>
                <th>星期二</th>
               
                <th>星期三</th>
                
                <th>星期四</th>
                
                <th>星期五</th>
                
                <th>星期六</th>
                 
                <th>星期日</th>
                
            </tr>
            <tr>
                <td>第1节</td>
                <th>)";
    String html32 = preferences1.getString("mon1");
    String html33 = "</th> <th>";
    String html34 = preferences1.getString("tue1");
    String html35 = "</th> <th>";
    String html36 = preferences1.getString("wed1");
    String html37 = "</th> <th>";
    String html38 = preferences1.getString("thu1");
    String html39 = "</th> <th>";
    String html310 = preferences1.getString("fri1");
    String html311 = "</th> <th>";
    String html312 = preferences1.getString("sat1");
    String html313 = "</th> <th>";
    String html314 = preferences1.getString("sun1");
    String html315 = "</th> </tr>";

    String html316 = R"(<tr>
                <td>第5节</td> <th>)";

    String html317 = preferences1.getString("mon5");
    String html318 = "</th> <th>";
    String html319 = preferences1.getString("tue5");
    String html320 = "</th> <th>";
    String html321 = preferences1.getString("wed5");
    String html322 = "</th> <th>";
    String html323= preferences1.getString("thu5");
    String html324= "</th> <th>";
    String html325 = preferences1.getString("fri5");
    String html326 = "</th> <th>";
    String html327 = preferences1.getString("sat5");
    String html328 = "</th> <th>";
    String html329 = preferences1.getString("sun5");
    String html330 = "</th> </tr>";

    String html331 = R"( </table>
    </form>
</body>
</html>)";
    request->send(200, "text/html", html31+html32+html33+html34+html35+html36+html37+html38+html39+html310+html311+html312+html313+html314+html315+html316+html317+html318+html319+html320+html321+html322+html323+html324+html325+html326+html327+html328+html329+html330+html331);
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
     
    if (request->hasParam("ssid",true) && request->hasParam("password",true)) {
         String ssid = request->getParam("ssid", true)->value();
        String password = request->getParam("password", true)->value();
    
       if (ssid.isEmpty() || password.isEmpty()) {
            request->send(200, "text/plain", "SSID and Password are required");
        } else {
            // wifi_ntpinit 是自定义的函数，用于初始化 WiFi
            wifi_ntpinit(ssid.c_str(), password.c_str());
            request->send(200, "text/plain", "WiFi credentials saved");
        }
    } else {
        request->send(200, "text/plain", "No SSID or Password provided");
    }
});




  server.on("/submit_grades", HTTP_POST, [](AsyncWebServerRequest *request){
    preferences1.begin("timetable", false);
    for (int i = 0; i < 14; i++) {
        if(request->hasParam(timetablegroup[i], true)) {
            String mid = request->getParam(timetablegroup[i], true)->value();
            preferences1.putString(timetablegroup[i], mid.isEmpty() ? "No Class" : mid);
        }
    }
    preferences1.end();
    request->send(200, "text/html", SUCCESS_HTML); // 使用统一成功页面
});


server.on("/ap_setup", HTTP_GET, [](AsyncWebServerRequest *request) {
    ap_prefs.begin("ap_config", true);
    String current_ssid = ap_prefs.getString("ssid", default_ap_ssid);
    String current_pwd = ap_prefs.getString("password", default_ap_password);
    ap_prefs.end();

    String html = R"(<!DOCTYPE html>
<html lang='zh'>
<head>
    <meta charset='UTF-8'>
    <title>热点设置</title>
    <style>
        body { font-family: Arial, sans-serif; padding: 20px; }
        label { display: block; margin: 10px 0; }
        input[type="text"], input[type="password"] { 
            width: 200px;
            padding: 5px;
            margin-bottom: 10px;
        }
    </style>
</head>
<body>
    <h1>修改热点设置</h1>
    <form action='/update_ap' method='post'>
        <label>新热点名称：</label>
        <input type='text' name='ap_ssid' value=')" 
        + current_ssid + R"(' required>
        
        <label>新密码（8-64字符）：</label>
        <input type='password' name='ap_password' 
               pattern='.{8,64}' 
               title='8-64个字符'
               value=')" + current_pwd + R"(' required>
        
        <input type='submit' value='保存'>
    </form>
</body>
</html>)";

    request->send(200, "text/html; charset=utf-8", html); // 明确指定编码
});

server.on("/update_ap", HTTP_POST, [](AsyncWebServerRequest *request){
    String new_ssid = request->getParam("ap_ssid", true)->value();
    String new_pwd = request->getParam("ap_password", true)->value();

    // 验证SSID长度
    if(new_ssid.length() < 3 || new_ssid.length() > 32) {
        // 使用 HTML 和 CSS 美化错误提示
        String errorHtml = R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        text-align: center;
                        padding: 50px;
                        background: #f0f0f0;
                    }
                    .error-box {
                        background: white;
                        padding: 30px;
                        border-radius: 10px;
                        box-shadow: 0 0 10px rgba(0,0,0,0.1);
                        display: inline-block;
                    }
                    .error-message {
                        color: #FF0000;
                        font-size: 24px;
                        margin-bottom: 20px;
                    }
                    .home-link {
                        display: inline-block;
                        margin-top: 20px;
                        padding: 10px 20px;
                        background: #4CAF50;
                        color: white;
                        text-decoration: none;
                        border-radius: 5px;
                    }
                </style>
            </head>
            <body>
                <div class="error-box">
                    <div class="error-message">The SSID must be 3-32 characters long</div>
                    <a href="/ap_setup" class="home-link">Back to AP Setup</a>
                </div>
            </body>
            </html>
        )";
        request->send(400, "text/html", errorHtml);
        return;
    }

    // 验证密码长度
    if(new_pwd.length() < 8 || new_pwd.length() > 64) {
        String errorHtml = R"(
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        text-align: center;
                        padding: 50px;
                        background: #f0f0f0;
                    }
                    .error-box {
                        background: white;
                        padding: 30px;
                        border-radius: 10px;
                        box-shadow: 0 0 10px rgba(0,0,0,0.1);
                        display: inline-block;
                    }
                    .error-message {
                        color: #FF0000;
                        font-size: 24px;
                        margin-bottom: 20px;
                    }
                    .home-link {
                        display: inline-block;
                        margin-top: 20px;
                        padding: 10px 20px;
                        background: #4CAF50;
                        color: white;
                        text-decoration: none;
                        border-radius: 5px;
                    }
                </style>
            </head>
            <body>
                <div class="error-box">
                    <div class="error-message">The password must be 8-64 characters long</div>
                    <a href="/ap_setup" class="home-link">Back to AP Setup</a>
                </div>
            </body>
            </html>
        )";
        request->send(400, "text/html", errorHtml);
        return;
    }

    // 保存新配置
    ap_prefs.begin("ap_config", false);
    ap_prefs.putString("ssid", new_ssid);
    ap_prefs.putString("password", new_pwd);
    ap_prefs.end();

    request->send(200, "text/html", SUCCESS_HTML); // 使用统一成功页面

    // 重启AP
    
    delay(5000);
    WiFi.softAPdisconnect(true);
    WiFi.softAP(new_ssid.c_str(), new_pwd.c_str());
    //ESP.restart(); // 执行重启
    

});

     server.begin();  // 启动 Web 服务器
     delay(2000);
     wifi_ntpinit("a","a"); //在启动服务器后再对WiFi设置进行初始化
     
}


String GetTimeTable(String WeekDay,String AMPM)             //输入星期和上下午两个参数，返回这两个参数在时间表中所对应的
{
    preferences1.begin("timetable", false);  // 打开一个名为"timetable"的命名空间，false表示以读写模式打开
    if(AMPM == "AM")          //如果当前时间是上午，则执行第一节课的判断
    {
       if(WeekDay == "Monday")
       {
            return preferences1.getString("mon1");
       }
       else if(WeekDay == "Tuesday")
       {
            return preferences1.getString("tue1");
       }
       else if(WeekDay == "Wednesday")
       {
            return preferences1.getString("wed1");
       }
       else if(WeekDay == "Thursday")
       {
            return preferences1.getString("thu1");
       }
       else if(WeekDay == "Friday")
       {
            return preferences1.getString("fri1");
       }
       else if(WeekDay == "Saturday")
       {
            return preferences1.getString("sat1");
       }
       else if(WeekDay == "Sunday")
       {
            return preferences1.getString("sun1");
       }
       
    }

    else if(AMPM == "PM")          //如果当前时间是下午，则执行第五节课的判断
    {
       if(WeekDay == "Monday")
       {
            return preferences1.getString("mon5");
       }
       else if(WeekDay == "Tuesday")
       {
            return preferences1.getString("tue5");
       }
       else if(WeekDay == "Wednesday")
       {
            return preferences1.getString("wed5");
       }
       else if(WeekDay == "Thursday")
       {
            return preferences1.getString("thu5");
       }
       else if(WeekDay == "Friday")
       {
            return preferences1.getString("fri5");
       }
       else if(WeekDay == "Saturday")
       {
            return preferences1.getString("sat5");
       }
       else if(WeekDay == "Sunday")
       {
            return preferences1.getString("sun5");
       }
       
    }
    return "";       // 添加一个默认的返回值，以确保函数在所有情况下都有返回值
}