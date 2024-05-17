

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
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(bz, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

}

void loop() {
  distance = do_KC();                                         // do khoang cach = distance;
  distance = constrain(distance, 0, 10);           // gioi han kc tu 0-10cm, thay doi gia tri cho phu hop 
  int food_amount = map(distance, 2, 10, 100, 0);  // convert luong thuc an tu khoang cach theo ti le %, vd: kc la 10(max) la khong con thuc an -> luong_thuc_an = 0

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
  int pre_time = millis();
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
