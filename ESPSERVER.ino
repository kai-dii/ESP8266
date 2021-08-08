 #include <ESP8266WiFi.h>
#include <espnow.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>


const char* ssid     = "TP-Link_C11F";
const char* password = "12746798";

const char* mqttServer = "61.58.248.108";
const int mqttPort = 1883;
const char* mqttUser = "Gym_mqtt";
const char* mqttPassword = "!zxc123zxc";
 const char* topic = "gym/data";
WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    
  }
}

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int id;
    int x;
    int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;

// Create an array with all the structures
struct_message boardsStruct[4] = {board1, board2,board3,board4};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
 
}
