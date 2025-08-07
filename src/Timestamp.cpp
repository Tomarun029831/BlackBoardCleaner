#include "../lib/Timestamp.hpp"
#include <cmath>

Timestamp create_timestamp(char day, unsigned int hour_minute) {
    Timestamp ts = {day, hour_minute};
    return ts;
}

void timestamp_next_day(struct Timestamp& ts) {
    if (ts.day == '6') {
        ts.day = '0';  // 土曜日の次は日曜日
    } else {
        ts.day++;
    }
}

void timestamp_prev_day(struct Timestamp& ts) {
    if (ts.day == '0') {
        ts.day = '6';  // 日曜日の前は土曜日
    } else {
        ts.day--;
    }
}

unsigned int timestamp_get_hour(const struct Timestamp& ts) {
    return ts.hour_minute / 100;
}

unsigned int timestamp_get_minute(const struct Timestamp& ts) {
    return ts.hour_minute % 100;
}

void timestamp_add_minutes(struct Timestamp& ts, int minutes) {
    int total_minutes = timestamp_get_hour(ts) * 60 + timestamp_get_minute(ts);
    total_minutes += minutes;

    while (total_minutes < 0) {
        total_minutes += 24 * 60;
        timestamp_prev_day(ts);
    }
    while (total_minutes >= 24 * 60) {
        total_minutes -= 24 * 60;
        timestamp_next_day(ts);
    }

    unsigned int hour = total_minutes / 60;
    unsigned int minute = total_minutes % 60;
    ts.hour_minute = hour * 100 + minute;
}

static constexpr unsigned int marginOfError = 1; // 1 minute

bool timestamp_compare_hour_minute(unsigned int current_hour_minute, unsigned int target_hour_minute){
    int cur_total = (current_hour_minute / 100) * 60 + (current_hour_minute % 100);
    int tar_total = (target_hour_minute / 100) * 60 + (target_hour_minute % 100);

    int sub = (cur_total - tar_total);
    return 0 <= sub && sub <= marginOfError;
}

