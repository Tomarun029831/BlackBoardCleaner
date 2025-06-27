#include "../lib/KIC.h"
#include "../lib/SPool.h"
// #include <HardwareSerial.h> // TODO: only for DEBUG

static Schedule* convertToSchedule(const String& block);

SPool* KIC::convertToSPool(String& kic) {
    SPool* pool = new SPool(nullptr, 0); // TODO: new -> malloc
    // Serial.print("convertToSPool() called with: ");
    // Serial.println(kic);

    // check KICEND at end of the string
    if (!kic.endsWith(KICEND)) {
      return nullptr;
    }

    // devide the string with KICSEGMENTCHAR to extract kicHeader
    unsigned int begin = 0;
    String kicHeader = kic.substring(begin, kic.indexOf(KICSEGMENTCHAR, begin));
    // kic += begin; // move kic pointer to next head of the block

    // devide the string with KICSEGMENTCHAR to extract timeToSyc
    begin = kic.indexOf(KICSEGMENTCHAR, begin+1) + 1;
    unsigned int timeToSyc = kic.substring(begin, kic.indexOf(KICSEGMENTCHAR, begin)).toInt(); // HACK: type diff(unsigned int vs long)
    // kic += begin; // move kic pointer to next head of the block

    // start loop to extract each schedule with KICSEGMENTCHAR
    unsigned int newCount = 0;
    Schedule *newSchedules = nullptr;
    // Serial.print("convertToSPool> header: ");
    // Serial.println(kicHeader);
    // Serial.print("convertToSPool> timeHeader: ");
    // Serial.println(timeToSyc);
    // Serial.println("convertToSPool> Schedules@");
    while (true) {
      if (!kic.substring(begin).compareTo("/")) { // 0: if String equals myString2
        // Serial.println("@");
        break;
      }
      begin = kic.indexOf(KICSEGMENTCHAR, begin+1) + 1;
      String scheduleBlock=kic.substring(begin, kic.indexOf(KICSEGMENTCHAR, begin));
      // Serial.print(">> ");
      // Serial.println(scheduleBlock);
      Schedule *schedule=convertToSchedule(scheduleBlock);
      // Serial.println("convertToSPool> converted(");
      // Serial.print("cmd=");
      // Serial.println(schedule->cmd);
      // Serial.println("hours@");
      // for(unsigned int i = 0; i < schedule->count; i++){
      //   Serial.println(schedule->hours[i]);
      // }
      // Serial.println("@");
      // Serial.print("count=");
      // Serial.println(schedule->count);
      // Serial.println(")");

      queueSchedule(schedule, pool);
    }

    return pool;
  }

static Schedule* convertToSchedule(const String& block) {
    // Serial.print("convertToSchedule called with: ");
    // Serial.println(block);
    char cmd = block.charAt(0);
    Schedule* schedule=new Schedule(cmd, nullptr, 0); // TODO: new -> malloc

    for (int i = 1; i + 4 <= block.length() + 1; i += 4) {
      String chunk = block.substring(i, i + 4);
      // Serial.print("convertToSchedule> queueHours call with ");
      // Serial.println(chunk);

      int hour = atoi(chunk.c_str());
      queueHours(hour, schedule);
    }

    // Serial.print("convertToSchedule> cmd=");
    // Serial.println(schedule->cmd);
    return schedule;
  }
