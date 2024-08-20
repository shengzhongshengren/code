/*ESP32自己创建一个局域网网络，并作为WebServer*/
#include <WiFi.h>

//响应头
String responseHeaders =
    String("") +
    "HTTP/1.1 200 OK\r\n" +
    "Content-Type: text/html\r\n" +
    "Connection: close\r\n" +
    "\r\n";
//网页
String myhtmlPage =
    String("") +
    "<!DOCTYPE html>"+
    "<html>"+
    "<head>"+
     "<title>ESP32 AP Web Server</title>"+
    "<meta charset= \"utf-8\">"+
    "<style>html{text-align:center;}</style>"+
    "</head>"+
    "<body><br>"+
    "<h2>ESP32 Web Server</h2>"+
    "<p>GPIO 4-\"+led1State+\"</p>"+
    "<p><a href=\"/LED1_on\"><button>Open</button></a></p>"+
    "<p><a href=\"/LED1_off\"><button>Close</button></a></p>"+
    "<p>GPIO 25-\"+led2State+\"</p>"+
    "<p><a href=\"/LED2_blink\"><button>Blink</button></a></p>"+
    "<p><a href=\"/LED1_off\"><button>Close</button></a></p>"+
    "</body></html>";

  String readString="";
  const char *ssid = "ESP32";
  const char *password = "12345678";
  
  WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(32,OUTPUT);
  digitalWrite(32,LOW);
  
  Serial.begin(115200);
  Serial.println("Code is Running!");
  WiFi.softAP(ssid,password);
  IPAddress myIP=WiFi.softAPIP();
  Serial.print("AP ip address:");
  Serial.println(myIP);
  
  server.begin();
}
void loop()
{
    WiFiClient client = server.available(); //尝试建立客户对象
    if (client)                             //如果当前有客户可用
    {
        boolean currentLineIsBlank = true;
        Serial.println("[Client connected]");

        while (client.connected()) //如果客户端建立连接
        {
            if (client.available()) //等待有可读数据
            {
                char c = client.read(); //读取一字节数据
                readString += c;        //拼接数据
                /************************************************/
                if (c == '\n' && currentLineIsBlank) //等待请求头接收完成(接收到空行)
                {
                    //比较接收到的请求数据
                    if (readString.startsWith("GET / HTTP/1.1")) //如果是网页请求
                    {
                        client.print(responseHeaders); //向客户端输出网页响应
                        client.print(myhtmlPage);      //向客户端输出网页内容
                        client.print("\r\n");
                        Serial.println("GET Request!");
                        break;  //不添加break则一直连在SEP32的服务器上
                    }
                    else if (readString.startsWith("GET /LED1_on")) //改变LED状态请求
                    {
                        digitalWrite(32,HIGH);
                        //client.print("LED has been turn on");
                        client.print(responseHeaders); //向客户端输出网页响应
                        client.print(myhtmlPage);      //向客户端输出网页内容，也可以输出其他网页
                        client.print("\r\n");
                        Serial.println("LED has been turn on");
                        break;  //不添加break则一直连在SEP32的服务器上
                    }
                    else if (readString.startsWith("GET /LED1_off")) //改变LED状态请求
                    {
                        digitalWrite(32,LOW);
                        //client.print("LED has been turn on");
                        client.print(responseHeaders); //向客户端输出网页响应
                        client.print(myhtmlPage);      //向客户端输出网页内容，也可以输出其他网页
                        client.print("\r\n");
                        Serial.println("LED has been turn off");
                        break;  //不添加break则一直连在SEP32的服务器上
                    }
                    else
                    {
                        client.print(responseHeaders); //向客户端输出网页响应
                        client.print(myhtmlPage);      //向客户端输出网页内容，也可以输出其他网页
                        client.print("\r\n");
                        Serial.println("Not Anything!");
                        break;  //不添加break则一直连在SEP32的服务器上
                    }
                }
                if (c == '\n')
                {
                    currentLineIsBlank = true; //开始新行
                }
                else if (c != '\r')
                {
                    currentLineIsBlank = false; //正在接收某行中
                }
                /************************************************/
            }
        }
        delay(1);      //等待客户完成接收
        client.stop(); //结束当前连接:
        Serial.println("[Client disconnected]");

        Serial.println(readString); //打印输出来自客户的数据
        readString = "";
    }
}
