#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>
#include <stdlib.h>
#include "lib/Schedule.h"
#include "lib/SPool.h"

// COM9    serial   Serial Port (USB) Arduino Mega or Mega 2560 arduino:avr:mega arduino:avr

/*
compile script:
arduino-cli compile --fqbn arduino:avr:mega .\BlackBoardCleaner
*/

/*
upload script:
arduino-cli upload -p COM9 --fqbn arduino:avr:mega .\BlackBoardCleaner
*/


/*
auto compile and upload:
arduino-cli compile --fqbn arduino:avr:mega .\BlackBoardCleaner && arduino-cli upload -p COM9 --fqbn arduino:avr:mega .\BlackBoardCleaner && 
arduino-cli monitor -p COM9 --config baudrate=9600
*/


/*
typedef struct {
  POINT **points;
  unsigned int count;
} PPOOL;

static void freePool(PPOOL *const pool);
static int pushPoint(const POINT *point, PPOOL *pool);
static int isConcluded(const POINT *point, const PPOOL *pool);

static void freePool(PPOOL *const pool) {
  free(pool->points);
  pool->points = NULL;
  pool->count = 0;
}

static int pushPoint(const POINT *point, PPOOL *pool) {
  POINT const **new_pool =
      realloc(pool->points, (pool->count + 1) * sizeof(POINT *));
  if (!new_pool) {
    return 0;
  }

  *(new_pool + pool->count) = point;
  pool->points = (POINT **)new_pool;
  pool->count++;

  return 1;
}

static int isConcluded(const POINT *point, const PPOOL *pool) {
  for (const POINT *const *it = (const POINT *const *)pool->points;
       it != (const POINT *const *)pool->points + pool->count; ++it) {
    if (*it == point) {
      return 1;
    }
  }
  return 0;
}
*/

class ScheduleGateway{
  public:
    virtual int available()=0;
    virtual String receiveString()=0;
    virtual void sendString(String str)=0;
    virtual Schedule* getSchedule() = 0;
    virtual unsigned char getWeek() = 0;
    virtual unsigned char getHour() = 0;
};

void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

class SerialGateway : public ScheduleGateway{
  public:
    SerialGateway(){}

    int available(){
      return Serial.available();
    }

  String receiveString() override {
    String result = "";
    while (true) {
      if (Serial.available() <= 0) continue;
      char c = Serial.read();
      if (c == 'K') {  // 'K' から開始と仮定
        result += c;
        break;
      }
    }
    while (true) {
      if (Serial.available() <= 0) continue;
      char c = Serial.read();
      result += c;
      if (c == '/') break;  // 終端文字
    }
    return result;
  }

    void sendString(String str){
      Serial.print(str);
    }

    Schedule* getSchedule() override{

      // String str = Serial.readStringUntil(4); // ^D = ASCII 4
      // digitalWrite(LED_BUILTIN, HIGH);
      // delay(200);
      // digitalWrite(LED_BUILTIN, LOW);
      // Serial.print("receiption: ");
      // Serial.println(str);

    }

  protected:
    unsigned char getWeek() override{
      return 0;
    }

    unsigned char getHour() override{
      return 0;
    }
};

class WirelessGateway : public ScheduleGateway{
  public:
    WirelessGateway(){}

    int available(){
      return true;
    }

    Schedule* getSchedule() override{
      return new Schedule('-', 0, 0);
    }

  protected:
    unsigned char getWeek() override{
      return 0;
    }

    unsigned char getHour() override{
      return 0;
    }
};

//ex) KIC:V1;1437;1090010001130;208000900;8;9;/
//ex) KIC:V1;1437;1_0900_1000_1130;2_0800_0900;8;9;/

ScheduleGateway* gateway;
#define KIC_END '/'

class KicParser {
public:
  static SPool* convertToSPool(String *kic) {
    SPool* pool = new SPool(nullptr, 0);
    Serial.print("convertToSPool() called with: ");
    Serial.println(*kic);

    // check '/' at end of the string
    if (!kic->endsWith("/")) {
      return nullptr;
    }

    // devide the string with ';' to extract kicHeader
    int sepIndex = kic->indexOf(';');
    String kicHeader = kic->substring(0, sepIndex);
    kic += sepIndex; // move kic pointer to next head of the block

    // devide the string with ';' to extract timeToSyc
    sepIndex = kic->indexOf(';');
    unsigned int timeToSyc = kic->substring(0, sepIndex).toInt(); // HACK: type diff(unsigned int vs long)
    kic += sepIndex; // move kic pointer to next head of the block

    // start loop to extract each schedule with ';'
    unsigned int newCount = 0;
    Schedule *newSchedules = nullptr;
    while (true) {
      if (!kic->compareTo("/")) { // 0: if String equals myString2
        break;
      }
      sepIndex = kic->indexOf(';');
      String scheduleBlock=kic->substring(0, sepIndex);
      Schedule *schedule=convertToSchedule(&scheduleBlock);
      queueSchedule(schedule, pool);
    }

    return pool;
  }

private:
  static Schedule* convertToSchedule(const String *block) {
    char cmd = block->charAt(0);
    ++block;
    Schedule* schedule=new Schedule(cmd, nullptr, 0);

    unsigned int *newHours = nullptr;
    unsigned int newCount = 0;
    for (int i = 0; i + 3 < block->length(); i += 4) {
      String chunk = block->substring(i, i + 3);
      int hour = atoi(chunk.c_str());
      queueHours(hour, schedule);
    }

    return schedule;
  }


  // static void parseScheduleBlock(const String& block) {
  //   if (block.length() < 5) return; // 曜日 + 最低1時刻
  //   int day = block.substring(0, 1).toInt();
  //   Serial.print("曜日: ");
  //   Serial.println(day);
  //
  //   String times = block.substring(1);
  //   for (int i = 0; i + 4 <= times.length(); i += 4) {
  //     String time = times.substring(i, i + 4);
  //     Serial.print(" - 時刻: ");
  //     Serial.println(time);
  //   }
  // }

  KicParser() = delete;
};

/*
typedef struct {
  POINT **points;
  unsigned int count;
} PPOOL;

static void freePool(PPOOL *const pool);
static int pushPoint(const POINT *point, PPOOL *pool);
static int isConcluded(const POINT *point, const PPOOL *pool);

static void freePool(PPOOL *const pool) {
  free(pool->points);
  pool->points = NULL;
  pool->count = 0;
}

static int pushPoint(const POINT *point, PPOOL *pool) {
  POINT const **new_pool =
      realloc(pool->points, (pool->count + 1) * sizeof(POINT *));
  if (!new_pool) {
    return 0;
  }

  *(new_pool + pool->count) = point;
  pool->points = (POINT **)new_pool;
  pool->count++;

  return 1;
}

static int isConcluded(const POINT *point, const PPOOL *pool) {
  for (const POINT *const *it = (const POINT *const *)pool->points;
       it != (const POINT *const *)pool->points + pool->count; ++it) {
    if (*it == point) {
      return 1;
    }
  }
  return 0;
}
*/

/*
String.end(String str) => '\0'
*/

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  gateway = new SerialGateway();
}

void loop() {
  clearSerialBuffer();
  if (gateway->available() <= 0) return;
  String str = gateway->receiveString();

  KicParser::convertToSPool(&str);
}
