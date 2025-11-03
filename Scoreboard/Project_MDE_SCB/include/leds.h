#include <Adafruit_NeoPixel.h>

// Configuration
#define PIN_1 6     // Pin for digits 1-4
#define PIN_2 7     // Pin for digits 5-8
#define PIN_3 8     // Pin for digits 9-11

#define DIGITS_1_4 84   // 4 digits × 7 segments × 3 LEDs
#define DIGITS_5_8 84   // 4 digits × 7 segments × 3 LEDs
#define DIGITS_9_11 63  // 3 digits × 7 segments × 3 LEDs

#define BRIGHTNESS 50   // Brightness level (0 to 255)

// Create NeoPixel objects for each pin
Adafruit_NeoPixel strip1(DIGITS_1_4, PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(DIGITS_5_8, PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(DIGITS_9_11, PIN_3, NEO_GRB + NEO_KHZ800);

// Digit-to-segment mapping (segments: A, B, C, D, E, F, G)
const bool digitMap[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Function to set a digit on a given strip
void displayDigit(Adafruit_NeoPixel &strip, int digitIndex, int digit, uint32_t color) {
  int ledsPerDigit = 21; // 7 segments × 3 LEDs
  int startIndex = digitIndex * ledsPerDigit;

  // Turn off all LEDs for this digit
  for (int i = startIndex; i < startIndex + ledsPerDigit; i++) {
    strip.setPixelColor(i, 0);
  }

  // Light up the segments for the selected digit
  for (int segment = 0; segment < 7; segment++) {
    if (digitMap[digit][segment]) {
      for (int led = 0; led < 3; led++) {
        strip.setPixelColor(startIndex + segment * 3 + led, color);
      }
    }
  }
  strip.show();
}

void ledtestsetup() {
  // Initialize NeoPixel strips
  strip1.begin();
  strip2.begin();
  strip3.begin();

  strip1.setBrightness(BRIGHTNESS);
  strip2.setBrightness(BRIGHTNESS);
  strip3.setBrightness(BRIGHTNESS);

  strip1.show();
  strip2.show();
  strip3.show();
}

void ledtest() {
  uint32_t color = strip1.Color(255, 0, 0); // Red color

  // Display numbers on digits 1-4 (Pin 6)
  for (int digitIndex = 0; digitIndex < 4; digitIndex++) {
    displayDigit(strip1, digitIndex, digitIndex + 1, color); // Display 1, 2, 3, 4
    delay(1000);
  }

  // Display numbers on digits 5-8 (Pin 7)
  for (int digitIndex = 0; digitIndex < 4; digitIndex++) {
    displayDigit(strip2, digitIndex, digitIndex + 5, color); // Display 5, 6, 7, 8
    delay(1000);
  }

  // Display numbers on digits 9-11 (Pin 8)
  for (int digitIndex = 0; digitIndex < 3; digitIndex++) {
    displayDigit(strip3, digitIndex, digitIndex + 9, color); // Display 9, 10, 11
    delay(1000);
  }
}
