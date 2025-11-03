#ifndef DISPLAYS_H
#define DISPLAYS_H

#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

LiquidCrystal_I2C menulcd(0x27,20,4);

byte customBar[] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000
};

void initlcddisplay (){
    menulcd.init();
    menulcd.backlight();
    menulcd.createChar(0, customBar);
}

Adafruit_7segment taptsdisplay = Adafruit_7segment();
Adafruit_7segment tbptsdisplay = Adafruit_7segment();
Adafruit_7segment gameclkdisplay = Adafruit_7segment();

void initI2C7seg (){
  taptsdisplay.begin(0x70);
  tbptsdisplay.begin(0x71);
  gameclkdisplay.begin(0x74);
  taptsdisplay.setBrightness(5);
  tbptsdisplay.setBrightness(5);
  gameclkdisplay.setBrightness(5);
}

struct SevenSeg {
  int a, b, c, d, e, f, g;
};

SevenSeg taflsdisplay, tbflsdisplay, tatosdisplay, tbtosdisplay, gameperdisplay;

void init7seg (){
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  for (int i=22; i<54; i++){
    pinMode(i, OUTPUT);
  }

  gameperdisplay.a = 2; gameperdisplay.b = 3; gameperdisplay.c = 4; gameperdisplay.d = 22; gameperdisplay.e = 23; gameperdisplay.f = 24; gameperdisplay.g = 25;
  taflsdisplay.a = 26; taflsdisplay.b = 28; taflsdisplay.c = 30; taflsdisplay.d = 32; taflsdisplay.e = 34; taflsdisplay.f = 36; taflsdisplay.g = 38;
  tatosdisplay.a = 27; tatosdisplay.b = 29; tatosdisplay.c = 31; tatosdisplay.d = 33; tatosdisplay.e = 35; tatosdisplay.f = 37; tatosdisplay.g = 39;
  tbflsdisplay.a = 40; tbflsdisplay.b = 42; tbflsdisplay.c = 44; tbflsdisplay.d = 46; tbflsdisplay.e = 48; tbflsdisplay.f = 50; tbflsdisplay.g = 52;
  tbtosdisplay.a = 41; tbtosdisplay.b = 43; tbtosdisplay.c = 45; tbtosdisplay.d = 47; tbtosdisplay.e = 49; tbtosdisplay.f = 51; tbtosdisplay.g = 53;
}

void setPins(SevenSeg display, bool a, bool b, bool c, bool d, bool e, bool f, bool g) {
    digitalWrite(display.a, a);
    digitalWrite(display.b, b);
    digitalWrite(display.c, c);
    digitalWrite(display.d, d);
    digitalWrite(display.e, e);
    digitalWrite(display.f, f);
    digitalWrite(display.g, g);
}

void displayNumber(int num, SevenSeg display) {
    switch (num) {
      case 0:
          setPins(display, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW);
          break;
      case 1:
          setPins(display, LOW, HIGH, HIGH, LOW, LOW, LOW, LOW);
          break;
      case 2:
          setPins(display, HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH);
          break;
      case 3:
          setPins(display, HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH);
          break;
      case 4:
          setPins(display, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH);
          break;
      case 5:
          setPins(display, HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH);
          break;
      case 6:
          setPins(display, HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH);
          break;
      case 7:
          setPins(display, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW);
          break;
      case 8:
          setPins(display, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH);
          break;
      case 9:
          setPins(display, HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH);
          break;
      default:
          setPins(display, LOW, LOW, LOW, LOW, LOW, LOW, HIGH);
          break;
    }
}

#endif // DISPLAYS_H