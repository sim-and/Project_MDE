#ifndef COMMUNICATION_H
#define COMMUNICATION_H

typedef struct time_data {
  int hour;
  int minute;
  int second;
  int year;
  int month;
  int day;
} time_data;

time_data currenttime;
void getcurrenttime(){
  currenttime.hour = 22;
  currenttime.minute = 05;
  currenttime.year = 2024;
  currenttime.month = 12;
  currenttime.day = 22;
}

#endif // COMMUNICATION_H
