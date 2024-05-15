
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trig = D4;
const int echo = D5;
const int bz = D6;
unsigned long startTime = 0;
bool isSignaling;
int distance;
void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(bz, OUTPUT);
}

void loop() {
  do_KC();                                         // do khoang cach = distance;
  distance = constrain(distance, 0, 10);           // gioi han kc tu 0-10cm, thay doi gia tri cho phu hop nghe
  int food_amount = map(distance, 0, 10, 100, 0);  // convert luong thuc an tu khoang cach theo ti le %, vd: kc la 10(max) la khong con thuc an -> luong_thuc_an = 0

  lcd.setCursor(2, 0);
  lcd.print("LUONG THUC AN");
  lcd.setCursor(7, 1);
  lcd.print("   ");
  lcd.print(food_amount);
  int pre_time = millis();
  if (food_amount == 0)  // het thuc an
  {
    // bao
    while (millis() - pre_time < 2000)  // cho bao 2s
    {
      digitalWrite(bz, 1);
      delay(100);
      digitalWrite(bz, 0);
      delay(100);
      //lcd.clear();
    }
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

