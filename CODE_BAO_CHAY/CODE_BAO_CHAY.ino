#define BLYNK_TEMPLATE_ID "TMPL6e9Yc36ah"
#define BLYNK_TEMPLATE_NAME "DO AN I"
#define BLYNK_AUTH_TOKEN "d1ZkHYR2V5KA13_--51dQyOK0uV83-Qb"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <string.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "CONGNH2727";    //Tên WiFi
char pass[] = "20192727";   //Mật khẩu WiFi
///////////////////////////////////////////////
#include <DHT.h>
#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 13
char TEMP = V0;
char HUMIDITY = V1;
DHT dht(DHTPIN, DHTTYPE);
float h, t, f;
///////////////////////////////////////////////
#include "PMS.h"
HardwareSerial SerialPMS(1);
PMS pms(SerialPMS);
PMS::DATA data;
#define RXD2 25
#define TXD2 26
// ESP32 --> Pantower PMS7003   
// 26    --> RX     truyền data cho PMS 7003
// 25    --> TX     nhận data từ PMS 7003
// GND   --> GND
int pm_2_5;
char PM = V5;
char BAOCHAY = V2;
///////////////////////////////////////////////
#define RELAYPIN  5 
#define COIPIN    21
unsigned long timer = 0;
#define delay 1000      // sau 1s update dữ liệu lên blynk,
void setup()
{
  Serial.begin(9600);
  SerialPMS.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(COIPIN, OUTPUT);
  pinMode(RELAYPIN, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
}


void loop()
{
  h = dht.readHumidity(); t = dht.readTemperature();
  Blynk.run();  
    if (pms.read(data))
  {
    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);
  if(data.PM_AE_UG_2_5>100)
    {
      digitalWrite(RELAYPIN, 1);
      tone(COIPIN,220,125);
      Blynk.virtualWrite(BAOCHAY, HIGH);
    }
    else
    {
      digitalWrite(RELAYPIN, 0);
      noTone(COIPIN);
      Blynk.virtualWrite(BAOCHAY, LOW);
    }
    Blynk.virtualWrite(TEMP, t);
    Blynk.virtualWrite(HUMIDITY, h);
    Blynk.virtualWrite(PM, data.PM_AE_UG_2_5);
  }
}
