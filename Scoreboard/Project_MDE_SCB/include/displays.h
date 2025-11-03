#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 26
#define DIO 25

TM1637Display display(CLK, DIO);

LiquidCrystal_I2C lcd(0x27,16,2);

uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };