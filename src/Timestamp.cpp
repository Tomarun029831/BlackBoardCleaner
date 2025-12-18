#include "../lib/Timestamp.hpp"
#include <Arduino.h>

unsigned int timestamp_get_hour(const struct Timestamp &ts) {
  return ts.hour_minute / 100;
}

unsigned int timestamp_get_minute(const struct Timestamp &ts) {
  return ts.hour_minute % 100;
}

const char *day_to_string(char day) {
  switch (day) {
  case '0':
    return "Sun";
  case '1':
    return "Mon";
  case '2':
    return "Tue";
  case '3':
    return "Wed";
  case '4':
    return "Thu";
  case '5':
    return "Fri";
  case '6':
    return "Sat";
  default:
    return "?";
  }
}

// Timestamp をシリアルに出力
void timestamp_print(const Timestamp &ts) {
  unsigned int hour = timestamp_get_hour(ts);
  unsigned int minute = timestamp_get_minute(ts);

  Serial.print(day_to_string(ts.day));
  Serial.print(" ");

  if (hour < 10)
    Serial.print("0");
  Serial.print(hour);
  Serial.print(":");

  if (minute < 10)
    Serial.print("0");
  Serial.print(minute);
  Serial.print(":");

  if (ts.second < 10)
    Serial.print("0");
  Serial.print(ts.second);
  Serial.print(".");

  if (ts.millisecond < 100)
    Serial.print("0");
  if (ts.millisecond < 10)
    Serial.print("0");
  Serial.println(ts.millisecond);
}

Timestamp create_timestamp(char day, unsigned int hour_minute) {
  Timestamp ts = {day, hour_minute, 0, 0};
  return ts;
}

void timestamp_next_day(struct Timestamp &ts) {
  if (ts.day == '6') {
    ts.day = '0'; // 土曜日の次は日曜日
  } else {
    ts.day++;
  }
}

void timestamp_prev_day(struct Timestamp &ts) {
  if (ts.day == '0') {
    ts.day = '6'; // 日曜日の前は土曜日
  } else {
    ts.day--;
  }
}

void timestamp_add_minutes(struct Timestamp &ts, int minutes) {
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

void timestamp_add_milliseconds(Timestamp &ts, long long ms) {
  // 現在の時刻を総ミリ秒に変換
  long long total_ms = (long long)timestamp_get_hour(ts) * 3600000LL +
                       (long long)timestamp_get_minute(ts) * 60000LL +
                       (long long)ts.second * 1000LL +
                       (long long)ts.millisecond;

  total_ms += ms;

  // 日をまたぐ場合の処理
  const long long DAY_IN_MS = 86400000LL; // 24 * 3600 * 1000

  while (total_ms < 0) {
    total_ms += DAY_IN_MS;
    timestamp_prev_day(ts);
  }

  while (total_ms >= DAY_IN_MS) {
    total_ms -= DAY_IN_MS;
    timestamp_next_day(ts);
  }

  // 時分秒ミリ秒に分解
  unsigned int hour = (total_ms / 3600000LL) % 24;
  unsigned int minute = (total_ms / 60000LL) % 60;
  ts.second = (total_ms / 1000LL) % 60;
  ts.millisecond = total_ms % 1000;

  ts.hour_minute = hour * 100 + minute;
}

static constexpr unsigned int marginOfError = 0; // 0 => 0 minute, 1 => 1 minute

bool timestamp_compare_hour_minute(unsigned int current_hour_minute,
                                   unsigned int target_hour_minute) {
  int cur_total =
      (current_hour_minute / 100) * 60 + (current_hour_minute % 100);
  int tar_total = (target_hour_minute / 100) * 60 + (target_hour_minute % 100);
  int sub = cur_total - tar_total;

  return (sub >= 0) && (sub <= (int)marginOfError);
}
