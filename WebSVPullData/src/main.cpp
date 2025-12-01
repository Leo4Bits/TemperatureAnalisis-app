
#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <WebServer.h>
#include "measurementError.cpp"

typedef unsigned long long ULL;
ULL cnt = 0;
float curSumHif = 0;
float curSSHif = 0;
float measureHifTimes = 0;

// setup DHT11
#define PinDHT11 4
#define DHTType DHT11
DHT dht11(PinDHT11,DHTType);

// Wifi in4
const char *ssid = "kinhbac";
const char *password = "nhatnam2006";

IPAddress local_ip(192,168,1,50);
IPAddress gateway(192,168,1,0);
IPAddress subnet(255,255,240,0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);

// Declare WebSV
WiFiServer webserver(80);

void setup(){
  Serial.begin(9600);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.config(local_ip,gateway,subnet,primaryDNS,secondaryDNS);
  WiFi.begin(ssid,password);
  Serial.printf("Connect to %s, connecting.",WiFi.SSID());
  while (WiFi.status()!= WL_CONNECTED)
  {
    delay(100);
    Serial.printf(".");
  }
  Serial.println();
  Serial.printf("Wifi connected to %s", WiFi.localIP());
  Serial.println();
  
  Serial.println("Webserver is starting. ");
  webserver.begin();
  Serial.println("DHT11 sensor is starting. ");
  dht11.begin();
  Serial.print("Select this link: ");
  Serial.println("http//10.242.1.174:5000/");
}

void loop(){
  
  WiFiClient Flask = webserver.available();
  if (Flask)
  {
    String EachLineData = "";
    while (Flask.connected())
    {
      if(Flask.available())
      {
        char c = Flask.read();
        if (c == '\n')
        {
          if (EachLineData.length() == 0)
          {
            Flask.println("HTTP/1.1 200 DONE");
            Flask.println("Content-type: application/json");
            Flask.println("Connection: close");
            Flask.println();

            float t = dht11.readTemperature(false);
            float h = dht11.readHumidity(false);
            
            if (!(isnan(t) || isnan(h))){
              float hif = dht11.computeHeatIndex(t,h,false);
              measureHifTimes += 1;
              curSumHif += hif;
              curSSHif += hif*hif;
              float measureError = StandardDeviation(curSumHif = curSumHif,curSSHif = curSSHif,measureHifTimes );
              if (!(isnan(measureError))){
                Flask.printf("{\"temp\":%.2f, \"hum\":%.2f, \"hif\":%.2f,  \"measureError\":%.2f,  \"measureHifTimes\":%.2f}\n", t, h, hif, measureError, measureHifTimes);
                Serial.printf("{\"temp\":%.2f, \"hum\":%.2f, \"hif\":%.2f, \"measureError\":%.2f,  \"measureHifTimes\":%.2f}\n", t, h, hif, measureError, measureHifTimes);
              }else{
                Serial.println("Failed to send because of the NaN measurement");
              }
            }else{
              Serial.println("Failed to send because of the NaN");
            }
            Flask.println();
            break;
          }else
          {
            EachLineData = "";
          }
        }else if (c != '\r')
        {
          EachLineData += c;
        }
      }
    }
    Flask.stop();
    Serial.println("Sent");
    ++cnt;
    Serial.printf("Send for  %dth time ", cnt);
  }
}

