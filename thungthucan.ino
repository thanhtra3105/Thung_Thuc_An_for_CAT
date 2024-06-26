#include <Servo.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Wire.h>                    //Gọi thư viện I2C để sử dụng các thư viện I2C
#include <LiquidCrystal_I2C.h>       //Thư viện LCD I2C

LiquidCrystal_I2C lcd(0x27, 16, 2);  //Khai báo địa chỉ I2C (0x27 or 0x3F) và LCD 16x02
Servo s;
int nut1 = 2;
int nut2 = 3;
int nut3 = 4;
int flag = 0;
int gio = 11, phut = 00;
int gio2 = 18, phut2 = 27;
int coi = 5;

const int trig = 6;
const int echo = 7;
int led1 = 8;
int led2 = 9;
unsigned long startTime = 0;
bool isSignaling;
int distance;
int value;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(nut1, INPUT_PULLUP);
  pinMode(nut2, INPUT_PULLUP);
  pinMode(nut3, INPUT_PULLUP);
  pinMode(coi, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  s.attach(11);
  s.write(35);
}

void loop() {
  tmElements_t tm;

  lcd.setCursor(2, 0);
  lcd.print("  ");
  if (RTC.read(tm)) {

    lcd.setCursor(4, 0);
    print2digits(tm.Hour);
    lcd.setCursor(6, 0);
    lcd.print(":");
    lcd.setCursor(7, 0);
    print2digits(tm.Minute);
    lcd.setCursor(9, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    print2digits(tm.Second);
    lcd.print("  ");

  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  hen_gio();
// hen gio trua
  if (gio == tm.Hour && phut == tm.Minute && tm.Second == 0) {
    s.write(125);
    digitalWrite(coi, 1);
    delay(5000);
    digitalWrite(coi, 0);
    s.write(35);
  }
// hen gio toi
  if (gio2 == tm.Hour && phut2 == tm.Minute && tm.Second == 0) {
    s.write(125);
    digitalWrite(coi, 1);
    delay(5000);
    digitalWrite(coi, 0);
    s.write(35);
  }

  distance = do_KC();                              // do khoang cach = distance;
  distance = constrain(distance, 2, 12);           // gioi han kc tu 0-10cm, thay doi gia tri cho phu hop
  int food_amount = map(distance, 2, 12, 100, 0);  // convert luong thuc an tu khoang cach theo ti le %, vd: kc la 10(max) la khong con thuc an -> luong_thuc_an = 0

  lcd.setCursor(0, 1);
  lcd.print("FOOD AMOUNT:");
  lcd.setCursor(15, 1);
  lcd.print("%");
  if (food_amount == 100) {
    lcd.setCursor(12, 1);
    lcd.print(food_amount);
  } else if (food_amount >= 10) {
    lcd.setCursor(12, 1);
    lcd.print(" ");
    lcd.print(food_amount);
  } else {
    lcd.setCursor(12, 1);
    lcd.print("  ");
    lcd.print(food_amount);
  }

  if (food_amount == 0)  // het thuc an
  {
    Serial.println("Den bao hieu dang ON");
    digitalWrite(led1, 1);
    digitalWrite(led2, 0);

  } else {
    Serial.println("Den bao hieu dang OFF");
    digitalWrite(led1, 0);
    digitalWrite(led2, 1);
  }  
 
  if(Serial.available())
  {
    int value = Serial.read();
    Serial.println(value );
    if(value == 1)
    {
      s.write(125);


    }
    else
      s.write(35);
  } 
  
}




void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.print('0');
  }
  lcd.print(number);
}

void hen_gio() {
  int temp = 0;
  int but3 = digitalRead(nut3);
  if (but3 == 0) {
    while (digitalRead(nut3) == 0)
      ;
    flag++;
  }
  temp = flag;
  Serial.print("flag:");
  Serial.println(flag);
  while (flag % 3 != 0) {
    if (flag % 2 != 0) {
      //lcd.setCursor(0, 0);
      //lcd.print("                   ");

      lcd.setCursor(2, 0);
      lcd.print("HEN GIO TRUA");
      lcd.setCursor(0, 1);
      lcd.print("    ");
      lcd.setCursor(4, 1);
      print2digits(gio);
      lcd.setCursor(6, 1);
      lcd.print(":");
      lcd.setCursor(7, 1);
      print2digits(phut);
      lcd.setCursor(9, 1);
      lcd.print("        ");
      if (digitalRead(nut1) == 0) {
        while (digitalRead(nut1) == 0)
          ;
        gio++;
        if (gio == 24)
          gio = 0;
      }
      if (digitalRead(nut2) == 0) {
        while (digitalRead(nut2) == 0)
          ;
        phut++;
        if (phut == 60)
          phut = 0;
      }
      Serial.println(gio);
      Serial.println(phut);
    } else {

      lcd.setCursor(2, 0);
      lcd.print("HEN GIO TOI ");
      lcd.setCursor(0, 1);
      lcd.print("    ");
      lcd.setCursor(4, 1);
      print2digits(gio2);
      lcd.setCursor(6, 1);
      lcd.print(":");
      lcd.setCursor(7, 1);
      print2digits(phut2);
      lcd.setCursor(9, 1);
      lcd.print("       ");
      if (digitalRead(nut1) == 0) {
        while (digitalRead(nut1) == 0)
          ;
        gio2++;
        if (gio2 == 24)
          gio2 = 0;
      }
      if (digitalRead(nut2) == 0) {
        while (digitalRead(nut2) == 0)
          ;
        phut2++;
        if (phut2 == 60)
          phut2 = 0;
      }
      Serial.println(gio2);
      Serial.println(phut2);
    }

    if (digitalRead(nut3) == 0) {
      while (digitalRead(nut3) == 0)
        ;
      flag++;
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
 