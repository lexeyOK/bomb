#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>


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

int pass_count = 10;

String pass_r;
String pass_h;



boolean pass_inp() {
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

    if (pass_h == pass_r) {
      pass_h = "";
      return 1;
    }
    else {
      lcd.clear();
      mp3_play(9);
      boom();
    }
  }
  // last_time = millis() - millis() % 100;
}
/* else if (millis() - millis() % 100 - last_time == 10000) {
  } // char key = keypad.getKey();
*/

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

    pass_r =  pass_h ;
    pass_h = "";
    return 1;
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






void timer() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("--:--");
  lcd.setCursor(5, 0);
  lcd.blink();
  int i = 0;
  for (i; i <= 1; i++) {
    char c = keypad.waitForKey();
    min_str += String(c);
    lcd.setCursor(i + 5, 0);
    lcd.print(c);
    _min = min_str.toInt();
  }
  i = 0;
  for (i; i <= 1; i++) {
    char a = keypad.waitForKey();
    sec_str += String(a);
    lcd.setCursor(i + 8, 0);
    lcd.print(a);
    sec = sec_str.toInt();
    sec++;
  }

  last_time = millis() - millis() % 100 - 1000;

  lcd.clear();
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
      --sec;
      if (_min == 1 && sec == 30) {
        mp3_play(2);
      }
      else if (_min == 1 && sec == 0) {
        mp3_play(3);
      }
      else if (_min == 0 && sec == 30) {
        mp3_play(4);
      }
      else if (_min == 0 && sec == 15) {
        mp3_play(5);
      }
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

      if (sec == 0 && _min == 0) {
        lcd.clear();
        boom();
      }
    }

  }
}

void boom() {
  mp3_play(10);
  mp3_play(6);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("YOU LOSE");
  delay(500);
  while (1) {
    //   tone(11, 1000, 5000); //взрыв
  }
}
void def() {
  mp3_play(7);
  lcd.clear();

  if (pass_inp()) {

    lcd.clear();
    digitalWrite(12, 0);
    digitalWrite(13, 1);
    lcd.setCursor(4, 0);
    lcd.print("YOU WIN!");
    mp3_play(8);
    delay(500);
    while (1) { }
    // tone(11, 3210, 500);

  }

}


void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  Serial.begin (9600);
  mp3_set_serial (Serial);
  mp3_set_volume (15);

  Wire.begin();
  Wire.beginTransmission(0x3F);

  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();

  start();

  last_time = millis() - millis() % 100 - 1000;

  timer();
}

void loop() {

}
