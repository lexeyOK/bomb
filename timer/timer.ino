#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <Keypad.h>

LiquidCrystal_PCF8574 lcd(0x3F);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 5, 4, 3, 2};
byte colPins[COLS] = { 9, 8, 7, 6};
unsigned long last_time = 0;
String sec_str;
String min_str;
int sec=10;
int _min=0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(0x3F);
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.clear();
  Serial.begin(9600);
  lcd.setCursor(5, 0);
  lcd.print("--:--");
  lcd.setCursor(5, 0);
  lcd.blink();
  for (int i; i <= 1; i++) {
    char c = keypad.waitForKey();
    min_str += String(c);
    lcd.setCursor(i + 5, 0);
    lcd.print(c);
    _min = min_str.toInt();
  }
 Serial.print("1");
  for (int i; i <= 1; i++) {
    char a = keypad.waitForKey();
    sec_str += String(a);
    lcd.setCursor(i + 8, 0);
    lcd.print(a);
    sec = sec_str.toInt();
  }
   Serial.print("2");
  last_time = millis() - millis() % 100 - 1000;
  lcd.clear();
}

void loop() {
  lcd.noBlink();
  if (millis() - millis() % 100 - last_time == 1000) {
    if (sec <= 0 && _min > 0) {
      --_min;
      sec = 59;
    }
    --sec;
    lcd.setCursor(5, 0);

    if (_min < 10 && sec < 10) {
      lcd.print("0" + String(_min) + ":"  + "0" + String(sec));
    }
    else if (sec < 10) {
      lcd.print(String(_min) + ":" + "0" + String(sec));
    }
    else if (_min < 10) {
      lcd.print("0" + String(_min) + ":"  + String(sec));
    }
    else {
      lcd.print(String(_min) + ":" + String(sec));
    }
    last_time = millis() - millis() % 100;
  }

  if (sec == 0 && _min == 0) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("time is over");
    delay(1000);
    while(1){}
  }
  
}
