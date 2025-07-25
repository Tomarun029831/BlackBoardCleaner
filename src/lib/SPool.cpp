#include "../../lib/SPool.hpp"
#include <stdlib.h>

SPool::SPool(Schedule** schedules, unsigned int count):schedules(schedules), count(count) {}

bool queueSchedule(Schedule* schedule, SPool* pool) {
  Schedule** newSchedules = (Schedule**)realloc(pool->schedules, (pool->count + 1) * sizeof(Schedule*));
  if (!newSchedules) return false;

  *(newSchedules + pool->count++) = schedule;
  pool->schedules = newSchedules;
  return true;
}

Schedule* dequeueSchedule(SPool* pool, bool& isSuccess) {
  if (pool->count == 0) {
    isSuccess = false;
    return nullptr;
  }

  Schedule* dequeued = new Schedule(*(pool->schedules[0]));

  // PERF: O(n)
  for (unsigned int i = 1; i < pool->count; ++i) {
    pool->schedules[i - 1] = pool->schedules[i];
  }

  Schedule** newSchedules = (Schedule**)realloc(pool->schedules, (pool->count - 1) * sizeof(Schedule*));
  if (!newSchedules && pool->count > 1) {
    isSuccess = false;
    return nullptr;
  }

  pool->schedules = newSchedules;
  pool->count--;
  isSuccess = true;
  return dequeued;
}

void freeSPool(SPool* pool) {
    if (pool == nullptr) return;

    if (pool->schedules != nullptr) {
        for(Schedule **it = pool->schedules; it != pool->schedules + pool->count; ++it){
            if(*it != nullptr) free(*it);
        }

        free(pool->schedules);
        pool->schedules = nullptr;
    }

    pool->count = 0; // TODO: unnecesary?
    free(pool);
    pool = nullptr;
}
