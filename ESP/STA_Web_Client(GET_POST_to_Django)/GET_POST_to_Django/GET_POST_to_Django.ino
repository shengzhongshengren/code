/*工作流程：先连接可以正常上网的路由器设备，再连接由花生壳映射出来的http域名和端口号（一般为80），和Django对应的服务器进行GET或者POST测试。*/
#include <WiFi.h>

const char *ssid = "qdmyp3";   //广域网路由器用户名
const char *password = "19920504y";  //密码

const char *host = "3303438ch7.qicp.vip"; //欲访问的域名，不带http

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
    WiFiClient client; //声明一个客户端对象，用于与服务器进行连接

    Serial.println("尝试访问服务器");
    if (client.connect(host, 80)) //80为一般网站的端口号
    {
        Serial.println("访问成功");

    //Get方式向服务器发送请求头，请求网页数据
//    String request = String("GET ") + "/one/?name=qqq&age=25" + " HTTP/1.1\r\n" + // 接口
//                     //"Content-Type: text/html;charset=utf-8\r\n" +   //不写就有默认值
//                     "Host: " + host  + "\r\n" + // 主机+端口
//                     //"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36\r\n" +  //不写就有默认值
//                     //"Connection: Keep Alive\r\n"; //Client不主动离线，一直保持在线状态
//                     "Connection: close\r\n"; // Client请求后主动离线
//    client.println(request);  // 访问接口。这里使用的是println即数据末尾再加 \r\n


    //POST方式向服务器发送请求头，请求网页数据
    String args = "name=qqq&age=22";  //POST方式要发送的数据包
    String request = String("POST ") + "/one/?name=qqq&age=25" + "/ HTTP/1.1\r\n" + // 接口
                   "Content-Type: application/x-www-form-urlencoded;\r\n" +  // form格式数据
                   "Host: " + host + "\r\n" +
                   "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36\r\n" +
                   "Content-Length: " + args.length() + "\r\n" + // 数据长度
                   "Connection: close\r\n\r\n" + // 避免占用过多资源, Keep Alive则会让ESP32作为的Client一直在线，跳不出while (client.connected() || client.available())循环
                   args + "\r\n"; // 数据体
    client.println(request); // println

        //以下代码将收到的网页数据按行打印输出
        //如果是常见的WEB Client（浏览器）的话会将收到的html文件渲染成我们一般看到的网页
        while (client.connected() || client.available()) //如果已连接或有收到的未读取的数据
        {
            if (client.available()) //如果有数据可读取
            {
                String line = client.readStringUntil('\n'); //按行读取数据
                Serial.println(line);
            }
        }

        client.stop(); //关闭当前连接
        Serial.println("Client is Quit!!!");
    }
    else
    {
        Serial.println("访问失败");
        client.stop(); //关闭当前连接
    }
    delay(10000);
}
