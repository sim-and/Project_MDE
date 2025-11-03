#include <DHT.h>
#include <DHT_U.h>
#include "RTClib.h"

#define DHTPIN 27     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void getReadings() {
  DateTime now = rtc.now();

  hour = now.hour();
  minute = now.minute();
  second = now.second();
  year = now.year();
  month = now.month();
  day = now.day();
}

void displayData() {
  DateTime now = rtc.now();

  // Get hour and minute
  int hour = now.hour();
  int minute = now.minute();

  // Read temperature and take only the integer part
  int temperatureData = (int)dht.readTemperature(); // Integer part of temperature

  // Format time for TM1637 display (e.g., 12:34 -> 1234)
  uint16_t timeData = (hour * 100) + minute;

  // Custom segment encoding for "C" (specific to TM1637)
  const uint8_t cSegment = 0b01110011; // Adjust based on your TM1637 library

  // Toggle display between time and temperature every 3 seconds
  static unsigned long lastSwitchTime = 0;
  static bool showTime = true;
  unsigned long currentMillis = millis();

  if (currentMillis - lastSwitchTime >= 30000) { // Switch every 30000 ms (30 seconds)
    showTime = !showTime;
    lastSwitchTime = currentMillis;
  }

  if (showTime) {
    // Display time with colon
    display.showNumberDecEx(timeData, 0b01000000, true); // Colon at middle position
  } else {
    // Clear the display to avoid any residual data
    //display.clear();

    // Custom segment encoding for "C"
    const uint8_t cSegment = 0b00111001; // Correct encoding for "C"

    // Prepare the segments array
    uint8_t segments[4] = {0, 0, 0, 0};  // Initialize with blank

    // Extract and set the first two digits of the temperature
    segments[0] = display.encodeDigit((temperatureData / 10) % 10); // Tens digit
    segments[1] = display.encodeDigit(temperatureData % 10);        // Units digit

    // Set the "C" on the last digit
    segments[3] = cSegment;

    // Display the segments
    display.setSegments(segments);
  }
}

