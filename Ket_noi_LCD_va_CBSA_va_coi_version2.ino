
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trig = D4;
const int echo = D5;
const int bz = D6;
unsigned long startTime = 0;
bool isSignaling;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(bz, OUTPUT);
}

void loop() {
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
  
  lcd.setCursor(0, 0);
  lcd.print("DISTANCE:");
  lcd.setCursor(14, 1);
  lcd.print("cm");
  
  if (distance < 10) {
    lcd.setCursor(11, 1);
    lcd.print("  ");
    lcd.print(distance);
    stopSignaling();  // Dừng báo hiệu nếu khoảng cách nhỏ hơn 10
    delay(1000);
  } else if (distance < 100) {
    lcd.setCursor(11, 1);
    lcd.print(" ");
    lcd.print(distance);
    unsigned long startTime = millis();
    isSignaling = true;
    while ((isSignaling = true)&&(millis() - startTime) < 2000) {
      Serial.println("Coi đang bật");
      digitalWrite(bz, HIGH);
      delay(200);
      digitalWrite(bz, LOW);
      delay(200);
      digitalWrite(bz, HIGH);
    } 
    isSignaling = false;
    
    //startSignaling();  // Bắt đầu báo hiệu nếu khoảng cách từ 10 đến 99
    delay(1000);
  } else {
    lcd.setCursor(11, 1);
    lcd.print(distance);
    // startSignaling();  // Bắt đầu báo hiệu nếu khoảng cách lớn hơn hoặc bằng 100
    // delay(1000);
  }
}

// void startSignaling() {
//  if (isSignaling = true ) {
//   while ((millis() - startTime) < 2000) {
//     Serial.println("Coi đang bật");
//     digitalWrite(bz, HIGH);
//     delay(200);
//     digitalWrite(bz, LOW);
//     delay(200);
//     digitalWrite(bz, HIGH);
//   } 
//   isSignaling = false;
//  }
// }
void stopSignaling() {
  digitalWrite(bz, LOW);
  Serial.println("Coi đã tắt");
  isSignaling = false;
}

