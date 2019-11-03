//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WiFiServer.h>
//#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Cập nhật thông tin
// Thông tin về wifi
#define STASSID "Mastermind"
#define STAPSK "12341234"

const char* ssid     = STASSID;
const char* password = STAPSK;

// Thông tin về MQTT Broker
#define mqtt_server "m21.cloudmqtt.com" // Thay bằng thông tin của bạn

#define mqtt_user "yiiwrsnc"    //Giữ nguyên nếu bạn tạo user là esp8266 và pass là 123456
#define mqtt_pwd "q2OoylDT-atF"

#define topicReqDevice1 "skyphantomhz/reqdevice1"
#define topicReqDevice2 "skyphantomhz/reqdevice2"
#define topicReqDevice3 "skyphantomhz/reqdevice3"
#define topicReqDevice4 "skyphantomhz/reqdevice4"
#define topicReqDevice5 "skyphantomhz/reqdevice5"
#define topicReqDevice6 "skyphantomhz/reqdevice6"

#define topicResDevice "skyphantomhz/resdevice"
#define readyDevice "skyphantomhz/readydevice"

#define D1 5
#define D2 4
#define D5 14
#define D6 12
#define D7 13
#define D8 15

const uint16_t mqtt_port = 19553; //Port của CloudMQTT

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  pinMode(D1,OUTPUT);pinMode(D5,OUTPUT);
  pinMode(D2,OUTPUT);pinMode(D6,OUTPUT);
  pinMode(D7,OUTPUT);pinMode(D8,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
}
// Hàm kết nối wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }
//  Serial.println();
  if (strcmp(topic,topicReqDevice1)==0){
    progressLed(D1,(char)payload[0]);
  }
  if (strcmp(topic,topicReqDevice2)==0){
    progressLed(D2,(char)payload[0]);
  }
  if (strcmp(topic,topicReqDevice3)==0){
    progressLed(D5,(char)payload[0]);
  }
 
  if (strcmp(topic,topicReqDevice4)==0) {
    progressLed(D6,(char)payload[0]);
  }
 
  if (strcmp(topic,topicReqDevice5)==0) {
    progressLed(D7,(char)payload[0]);
  }  
 
  if (strcmp(topic,topicReqDevice6)==0) {
    progressLed(D8,(char)payload[0]);
  }  
}

//điều khiển bật tắt đèn
void progressLed(int pin, char option){
  bool check=false;
  if(option=='0'){
    Serial.println("OFF LED");
    digitalWrite(pin, LOW);
    check=true;
  }else{
    if(option=='1'){
      Serial.println("ON LED");
      digitalWrite(pin, HIGH);
      check=true;
    }
  }
  if(check){
    client.publish(topicResDevice, "ok");
  }
}


// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      client.publish(readyDevice, "ok");
      // ... và nhận lại thông tin này
      client.subscribe(topicReqDevice1);client.subscribe(topicReqDevice4);
      client.subscribe(topicReqDevice2);client.subscribe(topicReqDevice5);
      client.subscribe(topicReqDevice3);client.subscribe(topicReqDevice6);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void loop() {
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Sau mỗi 2s sẽ thực hiện publish dòng hello world lên MQTT broker
//  long now = millis();
//  if (now - lastMsg > 2000) {
//    lastMsg = now;
//    ++value;
//    snprintf (msg, 75, "hello world #%ld", value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    client.publish(mqtt_topic_pub, msg);
//  }
}
