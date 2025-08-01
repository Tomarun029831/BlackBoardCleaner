#include "../lib/Schedule.hpp"
#include <stdlib.h>

Schedule::Schedule(const unsigned char cmd, unsigned int *const hours, const unsigned int count):cmd(cmd), hours(hours), count(count) {}

bool pushHour(const int hour, Schedule* schedule) {
  unsigned int* newHours = (unsigned int*)realloc(schedule->hours, (schedule->count + 1) * sizeof(unsigned int));
  if (!newHours) return false;

  newHours[schedule->count] = hour;
  schedule->hours = newHours;
  schedule->count++;
  return true;
}

unsigned int popHour(Schedule* schedule, bool& isSuccess) {
  if (schedule->count == 0) {
    isSuccess = false;
    return 0;
  }

  unsigned int poppedHour = schedule->hours[schedule->count - 1];

  unsigned int* newHours = NULL;
  if (schedule->count > 1) {
    newHours = (unsigned int*)realloc(schedule->hours, (schedule->count - 1) * sizeof(unsigned int));
    if (!newHours) {
      isSuccess = false;
      return 0;
    }
  }

  schedule->hours = newHours;
  schedule->count--;
  isSuccess = true;
  return poppedHour;
}
