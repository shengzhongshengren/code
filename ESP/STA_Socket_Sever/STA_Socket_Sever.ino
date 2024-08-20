/*工作流程：先连接可以正常上网的路由器（qdmyp3），并会返回ESP32在路由器中被分配的IP地址（192.168.x.xxx）,
创建服务器端口号8484，,用花生壳映射该IP地址和端口号成广域网的域名和端口号，利用SIM800L可以与之通讯，
loop函数中监听Client是否连接，连接后判断是否接受到的数据含有结束字符‘q’，有则回传给Client（SIM800L）*/
#include <WiFi.h>

const char *ssid = "qdmyp3";
const char *password = "19920504y";


WiFiClient client; //声明一个客户端对象，用于与服务器进行连接
WiFiServer server; //声明服务器对象

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
    
    server.begin(8484); //设置STA服务器启动监听端口号
}

void loop()
{
    WiFiClient client = server.available(); //尝试建立客户对象
    if (client) //如果当前客户可用
    {
        Serial.println("[Client connected]");
        String readBuff;
        while (client.connected()) //如果客户端处于连接状态
        {
            if (client.available()) //如果有可读数据
            {
                char c = client.read(); //读取一个字节
                                        //也可以用readLine()等其他方法
                readBuff += c;
                if(c == 'q') //接收到‘q’则(正规应该是判断是否接受‘\r‘，奈何手机APP发送不了)
                {
                    client.print("Client Send Data:" + readBuff); //向客户端发送
                    Serial.println("Received: " + readBuff); //从串口打印
                    readBuff = "";
                    //break; //跳出循环
                }
            }
        }
        client.stop(); //结束当前连接:
        Serial.println("[Client disconnected]");
    }
}
