#include "../lib/Schedule.h"
#include <stdlib.h>

Schedule::Schedule(const unsigned char cmd, unsigned int *const hours, const unsigned int count):cmd(cmd), hours(hours), count(count) {}

bool queueHours(const int hour, Schedule *schedule) {
  unsigned int* newHours = (unsigned int*)realloc(schedule->hours, (schedule->count + 1) * sizeof(unsigned int));
  if (!newHours) return false;
  *(newHours + schedule->count) = hour;

  schedule->hours = newHours;
  schedule->count++;
  return true;
}

unsigned int dequeueHour(Schedule *schedule, bool& isSuccess) {
  if (schedule->count == 0) {
    isSuccess = false;
    return 0;
  }

  unsigned int dequeuedHour = schedule->hours[0];

  // PERF: O(n)
  for (unsigned int i = 1; i < schedule->count; ++i) {
    schedule->hours[i - 1] = schedule->hours[i];
  }

  unsigned int* newHours = (unsigned int*)realloc(schedule->hours, (schedule->count - 1) * sizeof(unsigned int));
  if (!newHours && schedule->count > 1) {
    isSuccess = false;
    return 0;
  }

  schedule->hours = newHours;
  schedule->count--;
  isSuccess = true;
  return dequeuedHour;
}
