#ifndef _SPOOL_
#define _SPOOL_

#include "./Schedule.hpp"

struct SPool {
  SPool(Schedule** schedules = nullptr, unsigned int count = 0);

  Schedule** schedules;
  unsigned int count;
};

bool pushSchedule(Schedule* schedule, SPool* pool);
Schedule* popSchedule(SPool* pool, bool& isSuccess);
void freeSPool(SPool* pool);

#endif // !_SPOOL_
