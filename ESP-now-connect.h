#ifndef __ESP_NOW_CONNECT_H
#define __ESP_NOW_CONNECT_H
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac, const uint8_t* incomingData, int len);
void send(int a);
void ESP_INIT();
#endif