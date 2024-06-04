//Dung app blynk dieu khien 
#define BLYNK_TEMPLATE_ID "TMPL6BfFEr4LT"
#define BLYNK_TEMPLATE_NAME "Servo motor control using esp8266"
#define BLYNK_AUTH_TOKEN "tdqj0VBxG37GQWa0fooOaaBYCSTXof4y"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> //Thu vien dung de ket noi Wifi cho ESP8266
#include <BlynkSimpleEsp8266.h>

//Khai bao thu vien va bien cho servo
#include<Servo.h>
Servo servo;
//Thong tin ve mang Wifi:
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi 9TB";//Khai bao bien
char pass[] = "123456789";//Khai bao bien

void setup() {
  

  //set up servo va 
  servo.attach(D3);
  Blynk.begin(auth,ssid, pass);

  //Setup de ket noi Wifi:
  WiFi.mode(WIFI_STA);  //Thiet lap che do ket noi 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);  //Lenh ket noi Wifi che do mac dinh

  while (WiFi.status()  != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
  Serial.print("IP host: ");
  Serial.println(WiFi.localIP());
  delay(2000);

}

void loop() {
  //Run blynk
  Blynk.run();

  
//Dieu khien 
BLYNK_WRITE(V0)
{
  int s = param.asInt();
  Serial.println(s);
  if(s == 1){
    
    servo.write(180);
    delay(20);
  }
  else 
  {  servo.write(0);
     delay(20);
  }
 // Blynk.virtualWrite(V1, s);

}
