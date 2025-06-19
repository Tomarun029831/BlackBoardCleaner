#ifndef SCHEDULE_H
#define SCHEDULE_H

struct Schedule {
  Schedule(const unsigned char cmd='-', unsigned int *const hours=nullptr, const unsigned int count=0);

  unsigned char cmd;
  unsigned int* hours;
  unsigned int count;
};

bool queueHours(const int hour, Schedule *schedule);
unsigned int dequeueHour(Schedule *schedule, bool& isSuccess); // PERF: O(n)

#endif
