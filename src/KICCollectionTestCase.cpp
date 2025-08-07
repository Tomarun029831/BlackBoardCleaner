#include "../test/KICCollectionTestCase.hpp"
#include "../lib/CleaningDiagramCollection.hpp"
#include "../lib/KICCollection.hpp"
#include "../lib/Timestamp.hpp"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

namespace KICCollectionTestCase {
static bool assertString(const char *str1, const char *str2) {
  if (str1 == nullptr || str2 == nullptr)
    return false;
  return strcmp(str1, str2) == 0;
}

static bool assertDaySchedule(const CleaningDiagramCollection::DaySchedule *a,
                              const CleaningDiagramCollection::DaySchedule *b) {
  if (a == nullptr || b == nullptr)
    return false;
  // Remove header comparison as it doesn't exist in the struct
  if (a->length != b->length)
    return false;

  for (unsigned int i = 0; i < a->length; ++i) {
    if (a->hours[i] != b->hours[i])
      return false;
  }
  return true;
}

static bool
assertServerTimestamp(const Timestamp *timestamp1,
                      const Timestamp *timestamp2) {
  if (timestamp1 == nullptr || timestamp2 == nullptr)
    return false;

  // Compare day field
  if (timestamp1->day != timestamp2->day)
    return false;

  // Compare hour_minute field
  if (timestamp1->hour_minute != timestamp2->hour_minute)
    return false;

  return true;
}

static bool assertKICData(const KICCollection::KICData *data1,
                          const KICCollection::KICData *data2) {
  if (data1 == nullptr || data2 == nullptr) {

    return false;
  }

  // --- Check ServerTimestamp ---
  const auto &ts1 = data1->serverTimestamp;
  const auto &ts2 = data2->serverTimestamp;
  if (ts1.day != ts2.day || ts1.hour_minute != ts2.hour_minute) {

    return false;
  }

  // --- Check Board ---
  const auto &b1 = data1->board;
  const auto &b2 = data2->board;
  if (b1.height != b2.height || b1.width != b2.width) {

    return false;
  }

  // --- Check CleaningDiagram ---
  for (unsigned int i = 0; i < AMOUNT_OF_DAY_INWEEK; ++i) {
    const auto &s1 = data1->diagram.schedules[i];
    const auto &s2 = data2->diagram.schedules[i];
    if (s1.length != s2.length) {

      return false;
    }
    for (unsigned char j = 0; j < s1.length; ++j) {
      if (s1.hours[j] != s2.hours[j]) {

        return false;
      }
    }
  }

  return true;
}

bool testKICLexer() {

  String testString = "KIC:V3;01437;01140334;008001200;20700090011001300;/";

  const char *expected_kicHeader = KICCollection::KICVERSION;
  const char *expected_serverSendTime = "01437";
  const char *expected_boardSize = "01140334";
  const char *expected_cleanDiagram = "008001200;20700090011001300;/";

  char kicHeader[KICCollection::KICHEADERLENGTH];
  char serverSendTime[KICCollection::SERVERSENDTIMELENGTH];
  char boardSize[KICCollection::BOARDSIZELENGTH];
  char *cleanDiagram = nullptr;

  KICCollection::KICLexer(testString, kicHeader, serverSendTime, boardSize,
                          &cleanDiagram);

  bool isPassed = true;

  bool headerCheck = assertString(kicHeader, expected_kicHeader);
  if (!headerCheck)
    isPassed = false;

  bool timeCheck = assertString(serverSendTime, expected_serverSendTime);
  if (!timeCheck)
    isPassed = false;

  bool sizeCheck = assertString(boardSize, expected_boardSize);
  if (!sizeCheck)
    isPassed = false;

  bool diagramCheck = assertString(cleanDiagram, expected_cleanDiagram);
  if (!diagramCheck)
    isPassed = false;

  free(cleanDiagram);

  return isPassed;
}

bool testKICParser() {
  const char *test_kicHeader = KICCollection::KICVERSION;
  const char *test_serverSendTime = "01437";
  const char *test_boardSize = "01140334";
  const char *test_cleanDiagram = "008001200;20700090011001300;/";

  // 期待値の設定
  Timestamp expectedServerTimestamp;
  expectedServerTimestamp.day = '0';
  expectedServerTimestamp.hour_minute = 1437;

  KICCollection::Board expectedBoard;
  expectedBoard.height = 114;
  expectedBoard.width = 334;

  CleaningDiagramCollection::CleaningDiagram expected_diagram;
  CleaningDiagramCollection::initCleaningDiagram(expected_diagram);
  expected_diagram.schedules[0].hours[0] = 800;
  expected_diagram.schedules[0].hours[1] = 1200;
  expected_diagram.schedules[0].length = 2;
  expected_diagram.schedules[2].hours[0] = 700;
  expected_diagram.schedules[2].hours[1] = 900;
  expected_diagram.schedules[2].hours[2] = 1100;
  expected_diagram.schedules[2].hours[3] = 1300;
  expected_diagram.schedules[2].length = 4;

  KICCollection::KICData expected_kicData;
  expected_kicData.serverTimestamp.day = expectedServerTimestamp.day;
  expected_kicData.serverTimestamp.hour_minute =
      expectedServerTimestamp.hour_minute;
  expected_kicData.board = expectedBoard;
  expected_kicData.diagram = expected_diagram;

  KICCollection::KICData kicData;
  bool isCurrectParsed =
      KICCollection::KICParser(test_kicHeader, test_serverSendTime,
                               test_boardSize, test_cleanDiagram, kicData);

  bool isPassed = false;
  if (!isCurrectParsed) {
    isPassed = false;
  } else {
    bool assertResult = assertKICData(&kicData, &expected_kicData);

    isPassed = assertResult;
  }

  return isPassed;
}

void runAllTests() {
  Serial.println("KICCollectionTestCase");
  Serial.printf(" - testKICLexer %s\n", testKICLexer() ? "passed" : "failed");
  Serial.printf(" - testKICParser %s\n", testKICParser() ? "passed" : "failed");
}
}; // namespace KICCollectionTestCase
