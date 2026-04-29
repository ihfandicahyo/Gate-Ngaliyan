#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const int pinMotorNaik = 8;
const int pinMotorTurun = 9;
const int pinSensorIR = 2;
const int pinLimitAtas = 3;
const int pinLimitBawah = 4;

enum StatusGerbang { TERBUKA, TERTUTUP, MEMBUKA, MENUTUP };
StatusGerbang statusSaatIni = TERTUTUP;

void setup() {
  pinMode(pinMotorNaik, OUTPUT);
  pinMode(pinMotorTurun, OUTPUT);
  pinMode(pinSensorIR, INPUT);
  pinMode(pinLimitAtas, INPUT_PULLUP);
  pinMode(pinLimitBawah, INPUT_PULLUP);

  Wire.begin();
  rtc.begin();
}

void loop() {
  DateTime sekarang = rtc.now();

  if (sekarang.hour() == 22 && sekarang.minute() == 0 && statusSaatIni == TERTUTUP) {
    statusSaatIni = MEMBUKA;
  }
  else if (sekarang.hour() == 5 && sekarang.minute() == 0 && statusSaatIni == TERBUKA) {
    statusSaatIni = MENUTUP;
  }

  if (statusSaatIni == MEMBUKA) {
    if (digitalRead(pinLimitAtas) == HIGH) {
      digitalWrite(pinMotorNaik, HIGH);
      digitalWrite(pinMotorTurun, LOW);
    } else {
      digitalWrite(pinMotorNaik, LOW);
      digitalWrite(pinMotorTurun, LOW);
      statusSaatIni = TERBUKA;
    }
  }

  if (statusSaatIni == MENUTUP) {
    if (digitalRead(pinSensorIR) == LOW) {
      digitalWrite(pinMotorNaik, LOW);
      digitalWrite(pinMotorTurun, LOW);
    } else {
      if (digitalRead(pinLimitBawah) == HIGH) {
        digitalWrite(pinMotorNaik, LOW);
        digitalWrite(pinMotorTurun, HIGH);
      } else {
        digitalWrite(pinMotorNaik, LOW);
        digitalWrite(pinMotorTurun, LOW);
        statusSaatIni = TERTUTUP;
      }
    }
  }
}
