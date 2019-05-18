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

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long last_time = 0;
String sec_str;
String min_str;
int sec = 40;
int _min = 0;

int pass_count = 4;

String pass_r = "1245";
String pass_h;

boolean pass_ch() {
  while (1) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Enter pass");
    lcd.setCursor(3, 1);
    lcd.print("**********");
    lcd.setCursor(3, 1);
    lcd.blink();
    int i = 0;
    for (i; i < pass_count; i++) {
      char c = keypad.waitForKey();
      pass_h += String(c);
      lcd.setCursor(i + round(8 - pass_count / 2), 1);
      if (c == 'C') {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Enter pass");
        lcd.setCursor(3, 1);
        lcd.print("**********");
        lcd.setCursor(3, 1);
        i = -1;
        pass_h = "";
      }
      else {
        lcd.print(c);
      }
    }
    lcd.noBlink();
    // Serial.print(pass_h);

    if (pass_h == pass_r) {
      pass_h = "";
      return 1;
    }
    else {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("PASS_ERRORR");
      tone(11,900,500);
      delay(500);
      pass_h = "";
    }
  }
}

void start() {
  lcd.print("Press A to start");

  while (1) {
    if (keypad.waitForKey() == 'A') {

      lcd.clear();
      if (pass_ch() == 1) {
        lcd.clear();
        digitalWrite(12, 1);
        return;
      }
    }
  }
}

void def() {

  lcd.clear();

  if (pass_ch() == 1) {
    lcd.clear();
    digitalWrite(12, 0);
    digitalWrite(13, 1);
    lcd.setCursor(1, 1);
    lcd.print("YOU WIN!");
    tone(11, 3210, 500);

  }
  /*
    else if (millis() - millis() % 100 - last_time == 10000) {
    last_time = millis() - millis() % 100;
    }
  */
}
void timer() {
  while (1) {
    lcd.setCursor(1, 1);
    lcd.print("Press B to def");

    lcd.noBlink();
    if (millis() - millis() % 100 - last_time == 1000) {
      if (sec <= 0 && _min > 0) {
        --_min;
        sec = 59;
      }

      char key = keypad.getKey();
      if (key == 'B') {
        def();
      }

      
      tone(11, 1000, 20);//тиканье 
      --sec;
      lcd.setCursor(5, 0);
      if (_min < 10 && sec < 10) {
        lcd.print("0" + String(_min) + ":"  + "0" + String(sec));
        tone(11, 1000, 20);
        tone(11, 1000, 20);
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

      if (sec == 0 && _min == 0) {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("time is over");
        tone(11, 1000,5000); //взрыв
        break;
      }
    }

  }
}




void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Wire.begin();
  Wire.beginTransmission(0x3F);
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  Serial.begin(9600);
  start();
  last_time = millis() - millis() % 100 - 1000;  
  timer();
}

void loop() {
  
}
