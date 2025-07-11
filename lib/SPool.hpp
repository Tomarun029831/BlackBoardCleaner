#ifndef _SPOOL_
#define _SPOOL_

#include "./Schedule.hpp"

struct SPool {
  SPool(Schedule** schedules = nullptr, unsigned int count = 0);

  Schedule** schedules;
  unsigned int count;
};

bool queueSchedule(Schedule* schedule, SPool* pool);
Schedule* dequeueSchedule(SPool* pool, bool& isSuccess); // PERF: O(n)
void freeSPool(SPool* pool);

#endif // !_SPOOL_
