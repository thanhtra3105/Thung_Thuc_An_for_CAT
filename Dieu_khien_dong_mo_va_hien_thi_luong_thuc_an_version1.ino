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

//Hien thi luong thuc an và canh bao
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trig = D4;
const int echo = D5;
const int bz = D6;
int led1 = D7;
int led2 = D8;
unsigned long startTime = 0;
bool isSignaling;
int distance;
void setup() {
  //set up lcd
  lcd.init();
  lcd.backlight();//bat den nen

  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(bz, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

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

  //Run hien thi luong thuc an
  distance = do_KC();                                         // do khoang cach = distance;
  distance = constrain(distance, 0, 11);           // gioi han kc tu 0-10cm, thay doi gia tri cho phu hop 
  int food_amount = map(distance, 1, 11, 100, 0);  // convert luong thuc an tu khoang cach theo ti le %, vd: kc la 10(max) la khong con thuc an -> luong_thuc_an = 0

  lcd.setCursor(2, 0);
  lcd.print("LUONG THUC AN");
  lcd.setCursor(14, 1);
  lcd.print("%");
  if(food_amount == 100){
  lcd.setCursor(11, 1);
  lcd.print(food_amount);
  }else if(food_amount >= 10){
   lcd.setCursor(11, 1);
   lcd.print(" ");
   lcd.print(food_amount);
  }else {
    lcd.setCursor(11, 1);
    lcd.print("  ");
    lcd.print(food_amount);
  }
  Serial.println("Luong thuc an: ");
  Serial.println(food_amount);
  delay(500);
  //int pre_time = millis();
  if (food_amount == 1)  // het thuc an
  {
    // bao
    // while (millis() - pre_time < 2000)  // cho bao 2s
    // { Serial.println("Coi dang bao");
    //   digitalWrite(bz, 1);
    //   delay(100);
    //   digitalWrite(bz, 0);
    //   delay(100);
    //   //lcd.clear();
    // }
    //Bao bang led nhap nhay:
    Serial.println("Den bao hieu dang ON");
    digitalWrite(led1, 1);
    delay(1000);
    digitalWrite(led1, 0);
    delay(1000);
    digitalWrite(led2, 1);
    delay(1000);
    digitalWrite(led2, 0);
    delay(1000);
  }else{
   Serial.println("Den bao hieu dang OFF");
  }
}

int do_KC() {
  unsigned long duration;
  int distance;

  digitalWrite(trig, LOW);
  delayMicroseconds(3);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  delayMicroseconds(3);

  duration = pulseIn(echo, HIGH);
  distance = duration / 2 / 29.412;

  return distance;
}

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
