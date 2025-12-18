#ifndef _TIMESTAMP_
#define _TIMESTAMP_
#include <Arduino.h>

struct Timestamp {
  char day;                   // '0' - '6'
  unsigned int hour_minute;   // hhmm
  unsigned char second;       // 0-59
  unsigned short millisecond; // 0-999
};

Timestamp create_timestamp(char day, unsigned int hour_minute);
void timestamp_next_day(Timestamp &ts);
void timestamp_prev_day(Timestamp &ts);
void timestamp_add_minutes(Timestamp &ts, int minutes);
void timestamp_print(const Timestamp &ts);
void timestamp_add_milliseconds(Timestamp &ts, long long ms);
bool timestamp_compare_hour_minute(unsigned int current_hour_minute,
                                   unsigned int target_hour_minute);

#endif // !_TIMESTAMP_
