#include <esp_now.h>
#include <WiFi.h>
//#include "displays.h"

// REPLACE WITH THE MAC Address of your receiver
uint8_t peerMac[6] = { 0x08, 0xb6, 0x1f, 0xee, 0x47, 0x48 };

// Define variables to store DS3231 readings to be sent
int hour;
int minute;
int second;
int year;
int month;
int day;

// Define variables to store incoming game data
int tapts;
int tafls;
int tatos;
int tbpts;
int tbfls;
int tbtos;
int gameclk;
int gameper;


// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_time_data {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint16_t year;
  uint8_t month;
  uint8_t day;
} struct_time_data;


//Structure example to send data
//Must match the receiver structure
typedef struct struct_game_data {
  int tapts;
  int tafls;
  int tatos;
  int tbpts;
  int tbfls;
  int tbtos;
  int gameclk;
  int gameper;
} struct_game_data;

// Create a struct_message called timereadings to hold sensor readings
struct_time_data timereadings;

// Create a struct_message to hold incoming game data
struct_game_data incominggamedata;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect. Status:");
  lcd.setCursor(0,1);
  lcd.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == 0) {
    success = "Delivery Success :)";
  } else {
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&incominggamedata, incomingData, sizeof(incominggamedata));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  tapts = incominggamedata.tapts;
  tafls = incominggamedata.tafls;
  tatos = incominggamedata.tatos;
  tbpts = incominggamedata.tbpts;
  tbfls = incominggamedata.tbfls;
  tbtos = incominggamedata.tbtos;
  gameclk = incominggamedata.gameclk;
  gameper = incominggamedata.gameper;
}

void updateDisplay() {

  // Display Readings in Serial Monitor
  Serial.print("[RECEIVED] ");
  Serial.print(incominggamedata.gameclk);
  Serial.print(" ");
  Serial.print(incominggamedata.gameper);
  Serial.print(" ");
  Serial.print(incominggamedata.tapts);
  Serial.print(" ");
  Serial.print(incominggamedata.tafls);
  Serial.print(" ");
  Serial.print(incominggamedata.tatos);
  Serial.print(" ");
  Serial.print(incominggamedata.tbpts);
  Serial.print(" ");
  Serial.print(incominggamedata.tbfls);
  Serial.print(" ");
  Serial.print(incominggamedata.tbtos);
  Serial.println();
}