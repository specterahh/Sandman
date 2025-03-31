#include <esp_now.h>
#include <WiFi.h>

// 对端设备的 MAC 地址
uint8_t peerAddress[] = {0x34, 0x5F, 0x45, 0xE7, 0x63, 0xBC}; // 替换为枕头端的 MAC 地址


// 发送和接收的数据结构
typedef struct 
{
  char message = 'A';
} Data;

void send_massage(Data* Mydata );

Data myData;

// 发送回调函数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  log_e("Last Packet Send Status: ");
  if(status == ESP_NOW_SEND_SUCCESS)
  {
    log_e("Delivery Success");
  }
  else
  {
    delay(5);
    send_massage(&myData  );        //若是丢包了就重传一遍
    //log_e("Delivery Fail");
  }
}


// 接收回调函数
void OnDataRecv(const uint8_t *mac, const uint8_t* incomingData, int len) {
  Data * receivedData = (Data*)incomingData;
    Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Message: ");
  if(receivedData -> message == '1')
     {
        //再压力传感器到货后会接受到来自枕头端的数据来判断有没有人躺在枕头上，不过当前没有完成对应的程序
     }
  else
  {

  }
  
 

}



void send_massage(Data* Mydata  )//用于存储要发送的数据
{
  // 发送数据
  esp_err_t result = esp_now_send(peerAddress, (uint8_t *)Mydata, sizeof(Mydata));
  if (result == ESP_OK) {
    log_e("Sent with success");
  } else {
    log_e("Error sending the data");
  }
}

void send(int a)                        //若参数为1则发送‘1’到枕头端其余发送‘0’
{
    if(a == 1)
    {
        myData.message = '1';
    }
    else
    {
        myData.message = '0'; 
    }

    send_massage(&myData  );//调用上面的函数向另一终端发送信息
}

void ESP_INIT()
{
   Serial.begin(115200);
  //WiFi.mode(WIFI_STA);//这个函数再与闹钟ui的程序集成的时候似乎是不必要的，因为WiFi的初始化再UI启动的初期就执行过了

  // 初始化 ESP-NOW
  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }

  // 注册发送和接收回调函数
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // 添加对等设备
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    log_e("Failed to add peer");
    return;
  }
} 




