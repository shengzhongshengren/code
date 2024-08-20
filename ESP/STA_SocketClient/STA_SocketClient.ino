/*在局域网中连接上同一个路由器，路由器会给每一个设备分配一个临时IP地址(必须是同一个路由器下分配的IP地址)，例如在一个
 * 路由器上有两个设备，其一为ESP32设备，其二为手机（IP地址为192,168,1,102），由手机创建一个端口号为8585的
 * TCP Server，ESP32便可以在同一个路由器下寻找到IP地址为192,168,1,102端口号为8585的TCP服务器进行通讯；在广域网中
 定义好花生壳的端口和域名即可，至于花生壳映射的端口和域名无需非得是同一个路由器下分配的端口和域名*/
#include <WiFi.h>

const char *ssid = "qdmyp3";
const char *password = "19920504y";

/*局域网下的通讯*/
const IPAddress serverIP(192,168,3,32); //欲访问的地址(局域网内必须是同一个路由器下分配的IP地址)
uint16_t serverPort = 6666;         //服务器端口号(局域网)

/*广域网下的通讯*/
//const char *host = "3303438ch7.qicp.vip"; //欲访问的域名(广域网)[花生壳的]
//uint16_t serverPort = 31342;         //服务器端口号(广域网)[花生壳的]


WiFiClient client; //声明一个客户端对象，用于与服务器进行连接

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}

void loop()
{
    Serial.println("尝试访问服务器");
    if (client.connect(serverIP, serverPort)) //尝试访问目标地址（局域网下的通讯）
//    if (client.connect(host, serverPort)) //尝试访问目标地址（广域网下的通讯）
    {
        Serial.println("访问成功");

        client.print("Hello world!");                    //向服务器发送数据
        while (client.connected() || client.available()) //如果已连接或有收到的未读取的数据
        {
            if (client.available()) //如果有数据可读取
            {
                String line = client.readStringUntil('q'); //读取数据到q,是到q，不是遇到q结束，是遇到q停止，其中有超时设定，即使没有遇到q，超过一定时间后也会读取
                Serial.print("读取到数据：");
                Serial.println(line);
                client.write(line.c_str()); //将收到的数据回发
            }
        }
        Serial.println("关闭当前连接");
        client.stop(); //关闭客户端
    }
    else
    {
        Serial.println("访问失败");
        client.stop(); //关闭客户端
    }
    delay(5000);
}
