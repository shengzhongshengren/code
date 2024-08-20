#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "qdmyp3"; // 自家路由器的名称
const char *password = "19920504y";  // 自家路由器的密码

// MQTT Broker
const char *mqtt_broker = "3303438ch7.qicp.vip";  //花生壳映射后的broker地址
const char *topicSub = "esp32/Sub";  //ESP32的接收主题
const char *topicPub = "Server/Sub";   //ESP32的发布主题
const char *mqtt_username = "emqx";   //ESP32的用户名（可不写）
const char *mqtt_password = "public";  //ESP32的用户名密码（可不写）
const int mqtt_port = 31342;   //花生壳映射后的broker的端口号

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
   // Set software serial baud to 115200;
   Serial.begin(115200);
   // connecting to a WiFi network
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.println("Connecting to WiFi..");
   }
   Serial.println("Connected to the WiFi network");
   //connecting to a mqtt broker
   client.setServer(mqtt_broker, mqtt_port);
   client.setCallback(callback);
   while (!client.connected()) {
       const char *client_id = "esp32-client";
       //client_id += String(WiFi.macAddress());
       Serial.println("Connecting to public emqx mqtt broker.....");
       if (client.connect(client_id, mqtt_username, mqtt_password, NULL, 0 ,false, NULL, true)) {
           Serial.println("Public emqx mqtt broker connected");
       } else {
           Serial.print("failed with state ");
           Serial.print(client.state());
           delay(2000);
       }
   }
   // publish and subscribe
   client.publish(topicPub, "hello emqx");  //向Django发布主题
   client.subscribe(topicSub);    //接收主题
}

void callback(char *topic, byte *payload, unsigned int length) {
   Serial.print("Message arrived in topic: ");
   Serial.println(topic);
   Serial.print("Message:");
   for (int i = 0; i < length; i++) {
       Serial.print((char) payload[i]);
   }
   client.publish(topicPub, "I Received your Msg! Thank you!");   //向Django发布主题
   Serial.println();
   Serial.println("-----------------------");
}

void loop() {
   client.loop();   //巡回接收主题，client.loop();并不是死循环，可以在loop函数中进行其他动作，例如client.publish(topicPub, "hello emqx");,或者在callback(...)函数中回传client.publish信息
   //Serial.println("-----------------------");   
   //delay(1000);
}
