#include <WiFi.h>

const char *ssid = "ESP32";
const char *password = "12345678";

WiFiServer server; //声明服务器对象

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    WiFi.softAP(ssid, password);  //设置AP用户名和密码
    IPAddress myIP=WiFi.softAPIP();   //查询AP主机的IP地址
    Serial.print("IP Address:");
    Serial.println(myIP);  //打印
    server.begin(8888); //设置AP服务器启动监听端口号
    delay(500);
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
                if(c == '\n') //接收到‘q’则(正规应该是判断是否接受‘\n‘，奈何手机APP发送不了)
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
