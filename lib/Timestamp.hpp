#ifndef _TIMESTAMP_
#define _TIMESTAMP_
#include <Arduino.h>

struct Timestamp{
    char day; // '0' - Sunday, '1' - Monday, '2' - Tuesday, '3' - Wednesday, '4' - Thursday, '5' - Friday, '6' - Saturday
    unsigned int hour_minute; // hour_minute = 2313 -> 23:13
};

Timestamp create_timestamp(char day, unsigned int hour_minute);
void timestamp_next_day(Timestamp& ts);
void timestamp_prev_day(Timestamp& ts);
void timestamp_add_minutes(Timestamp& ts, int minutes);
bool timestamp_compare_hour_minute(unsigned int current_hour_minute, unsigned int target_hour_minute);


#endif // !_TIMESTAMP_
