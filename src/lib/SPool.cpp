#include "../../lib/SPool.hpp"
#include <stdlib.h>

SPool::SPool(Schedule** schedules, unsigned int count):schedules(schedules), count(count) {}

bool pushSchedule(Schedule* schedule, SPool* pool) {
  Schedule** newSchedules = (Schedule**)realloc(pool->schedules, (pool->count + 1) * sizeof(Schedule*));
  if (!newSchedules) return false;

  newSchedules[pool->count] = schedule;
  pool->schedules = newSchedules;
  pool->count++;
  return true;
}

Schedule* popSchedule(SPool* pool, bool& isSuccess) {
  if (pool->count == 0) {
    isSuccess = false;
    return nullptr;
  }

  Schedule* popped = pool->schedules[pool->count - 1];

  Schedule** newSchedules = NULL;
  if (pool->count > 1) {
    newSchedules = (Schedule**)realloc(pool->schedules, (pool->count - 1) * sizeof(Schedule*));
    if (!newSchedules) {
      isSuccess = false;
      return nullptr;
    }
  }

  pool->schedules = newSchedules;
  pool->count--;
  isSuccess = true;
  return popped;
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
