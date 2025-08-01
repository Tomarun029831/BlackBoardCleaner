// #include "../../lib/KIC.hpp"
// #include "../../lib/SPool.hpp"
// #include <HardwareSerial.h> // TODO: only for DEBUG
//
// // KIC:V1;01437;01140334;008001200;20700090011001300;/
//
// static Schedule* convertToSchedule(const String& block);
//
// KIC::KICINFO* KIC::convertToKIC(const String& kic) {
//     KIC::KICINFO* convertedKIC = (KIC::KICINFO*) malloc(sizeof(KIC::KICINFO));
//     if (convertedKIC == NULL) return NULL;
//
//     convertedKIC->spool = (SPool *)malloc(sizeof(SPool));
//     if (convertedKIC->spool == NULL) return NULL;
//
//     Serial.print("convertToSPool() called with: ");
//     Serial.println(kic);
//
//     // check KICEND at end of the string
//     if (!kic.endsWith(KICEND)) {
//       return nullptr;
//     }
//
//     // devide the string with KICSEGMENTCHAR to extract kicHeader
//     unsigned int begin = 0;
//     String kicHeader = kic.substring(begin, kic.indexOf(KICSEGMENTCHAR, begin));
//
//     // devide the string with KICSEGMENTCHAR to extract timeToSyc
//     begin = kic.indexOf(KICSEGMENTCHAR, begin) + 1;
//     String timeToSyc = kic.substring(begin, kic.indexOf(KICSEGMENTCHAR, begin));
//     convertedKIC->timestamp = convertToSchedule(timeToSyc);
//
//     unsigned int blackBoardHeight_cm = kic.substring(begin, begin + 3).toInt();
//     convertedKIC->board.height = blackBoardHeight_cm;
//     begin += 4;
//     unsigned int blackBoardWidth_cm = kic.substring(begin, begin + 3).toInt();
//     convertedKIC->board.width = blackBoardWidth_cm;
//     begin += 4;
//
//     // kic += begin; // move kic pointer to next head of the block
//     Serial.print("convertToSPool> header: ");
//     Serial.println(kicHeader);
//
//     Serial.print("convertToSPool> timeHeader: ");
//     Serial.println(timeToSyc);
//
//     Serial.print("convertToSPool> blackBoard height, width: ");
//     Serial.print(blackBoardHeight_cm);
//     Serial.print(", ");
//     Serial.println(blackBoardWidth_cm);
//
//     Serial.println("convertToSPool> Schedules@");
//
//     // start loop to extract each schedule with KICSEGMENTCHAR
//     unsigned int newCount = 0;
//     Schedule *newSchedules = nullptr;
//
//     while (true) {
//       if (!kic.substring(begin).compareTo("/")) { // 0: if String equals myString2
//         // Serial.println("@");
//         break;
//       }
//       begin = kic.indexOf(KICSEGMENTCHAR, begin+1) + 1;
//       String scheduleBlock = kic.substring(begin, kic.indexOf(KICSEGMENTCHAR, begin));
//
//       Serial.print(">> ");
//       Serial.println(scheduleBlock);
//
//       Schedule *schedule = convertToSchedule(scheduleBlock);
//
//       Serial.println("convertToSPool> converted(");
//       Serial.print("cmd=");
//       Serial.println(schedule->cmd);
//       Serial.println("hours@");
//       for(unsigned int i = 0; i < schedule->count; i++){
//         Serial.println(schedule->hours[i]);
//       }
//       Serial.println("@");
//       Serial.print("count=");
//       Serial.println(schedule->count);
//       Serial.println(")");
//
//       pushSchedule(schedule, convertedKIC->spool);
//     }
//
//     return convertedKIC;
//   }
//
// static Schedule* convertToSchedule(const String& block) {
//     // Serial.print("convertToSchedule called with: ");
//     // Serial.println(block);
//     char cmd = block.charAt(0);
//
//     Schedule* schedule = (Schedule*) malloc(sizeof(Schedule));
//     if (!schedule) return nullptr;
//
//     schedule->cmd = cmd;
//     schedule->hours = nullptr;
//     schedule->count = 0;
//
//     for (int i = 1; i + 4 <= block.length() + 1; i += 4) {
//       String chunk = block.substring(i, i + 4);
//       // Serial.print("convertToSchedule> queueHours call with ");
//       // Serial.println(chunk);
//
//       int hour = atoi(chunk.c_str());
//       pushHour(hour, schedule);
//     }
//
//     // Serial.print("convertToSchedule> cmd=");
//     // Serial.println(schedule->cmd);
//     return schedule;
//   }
