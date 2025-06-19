#ifndef SPOOL_H
#define SPOOL_H

#include "./Schedule.h"

struct SPool {
  SPool(Schedule** schedules = nullptr, unsigned int count = 0);

  Schedule** schedules;
  unsigned int count;
};

bool queueSchedule(Schedule* schedule, SPool* pool);
Schedule* dequeueSchedule(SPool* pool, bool& isSuccess);

#endif
