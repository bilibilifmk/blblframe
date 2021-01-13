#define FS_CONFIG
//兼容1.1.x版本库
#include <wifi_link_tool.h>
#include <ArduinoJson.h>
#include <String.h>
unsigned char LED_0F[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0xFF, 0xbf };
unsigned char LED[8]; //用于LED的4位显示缓存
int SCLK = 13;
int RCLK = 12;
int DIO = 14;

unsigned long UID = 14010836; //udi设置

const long interval = 30000; //刷新时间设置
unsigned long previousMillis = 0;
String disp = "";//临时储存粉丝数量
void setup() {
  //配置74锁存器脚位
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  //初始化
  LED[0] = 15;
  LED[1] = 15;
  LED[2] = 15;
  LED[3] = 15;
  LED[4] = 15;
  LED[5] = 15;
  LED[6] = 15;
  LED[7] = 15;
  //刷新到led
  LED8_Display();


  
  rstb = D3;
  //重置io
  stateled = D4;
  //指示灯io
  Hostname = "哔哩哔哩粉丝数显";
  //设备名称 允许中文名称 不建议太长
  wxscan = true;
  //是否被小程序发现设备 开启意味该设备具有后台 true开启 false关闭
  Serial.begin(115200);
  Serial.println("");

  
  load();
 
  //获取数据
  dy();
}

void loop() {
  pant();
  //刷新显示
  LED8_Display();
  //延迟微秒 防止闪烁
  delayMicroseconds(1);
  //时间延时
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //刷新数据
    dy();
  }
}




//获取粉丝数据及网络ip

void dy (void) {
  String  disp = "";
  if (UID != 0) {
    DynamicJsonBuffer jsonBuffer;
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://api.bilibili.com/x/relation/stat?vmid=" + String(UID))) {
      int httpCode = http.GET();
      String payload = http.getString();
      http.end();
      JsonObject& res_json = jsonBuffer.parseObject(payload);
      String follower = res_json["data"]["follower"];
      disp = follower;
      Serial.println("目前粉丝：" + String(disp));
    }
    leddp(disp);
  }
  else {
    IPAddress ips;
    ips = WiFi.localIP();
    LED[4] = 11;
    LED[3] = 11;
    String c = String(ips[2]);
    String d = String(ips[3]);
    Serial.println("当前局域网主机位：" + c + "  " + d);
    if (c.length() == 1) {
      LED[7] = 11;
      LED[6] = 11;
      LED[5] = c.toInt();
    }
    if (c.length() == 2) {
      LED[7] = 11;
      LED[6] = c.toInt() / 10 % 10;
      LED[5] = c.toInt() % 10 ;
    }
    if (c.length() == 3) {
      LED[7] = c.toInt() / 100 % 10;
      LED[6] = c.toInt() / 10 % 10;
      LED[5] = c.toInt() % 10 ;

    }
    if (d.length() == 1) {
      LED[2] = 11;
      LED[1] = 11;
      LED[0] = d.toInt();
    }
    if (d.length() == 2) {
      LED[2] = 11;
      LED[1] = d.toInt() / 10 % 10;
      LED[0] = d.toInt() % 10 ;
    }
    if (d.length() == 3) {
      LED[2] = d.toInt() / 100 % 10;
      LED[1] = d.toInt() / 10 % 10;
      LED[0] = d.toInt() % 10 ;
    }
  }
}



//对粉丝数据进行分割

void leddp(String leddps) {
  switch (leddps.length()) {
    case 1:
      LED[7] = 15;
      LED[6] = 15;
      LED[5] = 15;
      LED[4] = leddps.toInt();
      LED[3] = 15;
      LED[2] = 15;
      LED[1] = 15;
      LED[0] = 15;
      break;
    case 2:
      LED[7] = 11;
      LED[6] = 11;
      LED[5] = 11;
      LED[4] = leddps.charAt(0) - 48;
      LED[3] = leddps.charAt(1) - 48;
      LED[2] = 11;
      LED[1] = 11;
      LED[0] = 11;
      break;
    case 3:
      LED[7] = 15;
      LED[6] = 15;
      LED[5] = 15;
      LED[4] = leddps.charAt(0) - 48;
      LED[3] = leddps.charAt(1) - 48;
      LED[2] = leddps.charAt(2) - 48;
      LED[1] = 15;
      LED[0] = 15;
      break;
    case 4:
      LED[7] = 11;
      LED[6] = 11;
      LED[5] = leddps.charAt(0) - 48;
      LED[4] = leddps.charAt(1) - 48;
      LED[3] = leddps.charAt(2) - 48;
      LED[2] = leddps.charAt(3) - 48;
      LED[1] = 11;
      LED[0] = 11;
      break;
    case 5:
      LED[7] = 15;
      LED[6] = leddps.charAt(0) - 48;
      LED[5] = leddps.charAt(1) - 48;
      LED[4] = leddps.charAt(2) - 48;
      LED[3] = leddps.charAt(3) - 48;
      LED[2] = leddps.charAt(4) - 48;
      LED[1] = 15;
      LED[0] = 15;
      break;
    case 6:
      LED[7] = 11;
      LED[6] = leddps.charAt(0) - 48;
      LED[5] = leddps.charAt(1) - 48;
      LED[4] = leddps.charAt(2) - 48;
      LED[3] = leddps.charAt(3) - 48;
      LED[2] = leddps.charAt(4) - 48;
      LED[1] = leddps.charAt(5) - 48;
      LED[0] = 11;
      break;
    case 7:
      LED[7] = 15;
      LED[6] = leddps.charAt(0) - 48;
      LED[5] = leddps.charAt(1) - 48;
      LED[4] = leddps.charAt(2) - 48;
      LED[3] = leddps.charAt(3) - 48;
      LED[2] = leddps.charAt(4) - 48;
      LED[1] = leddps.charAt(5) - 48;
      LED[0] = leddps.charAt(6) - 48;
      break;
    case 8:
      LED[7] = leddps.charAt(0) - 48;
      LED[6] = leddps.charAt(1) - 48;
      LED[5] = leddps.charAt(2) - 48;
      LED[4] = leddps.charAt(3) - 48;
      LED[3] = leddps.charAt(4) - 48;
      LED[2] = leddps.charAt(5) - 48;
      LED[1] = leddps.charAt(6) - 48;
      LED[0] = leddps.charAt(7) - 48;
      break;

  }





}

//led数据传输

void LED8_Display (void)
{
  unsigned char *led_table;
  unsigned char i;
  //显示第1位
  led_table = LED_0F + LED[0];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x01);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第2位
  led_table = LED_0F + LED[1];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x02);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第3位
  led_table = LED_0F + LED[2];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x04);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第4位
  led_table = LED_0F + LED[3];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x08);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第5位
  led_table = LED_0F + LED[4];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x10);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第6位
  led_table = LED_0F + LED[5];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x20);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第7位
  led_table = LED_0F + LED[6];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x40);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  //显示第8位
  led_table = LED_0F + LED[7];
  i = *led_table;
  LED_OUT(i);
  LED_OUT(0x80);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
}

void LED_OUT(unsigned char X)
{
  unsigned char i;
  for (i = 8; i >= 1; i--)
  {
    if (X & 0x80)
    {
      digitalWrite(DIO, HIGH);
    }
    else
    {
      digitalWrite(DIO, LOW);
    }
    X <<= 1;
    digitalWrite(SCLK, LOW);
    digitalWrite(SCLK, HIGH);
  }
}
