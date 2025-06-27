#include <Arduino.h>
// #include <esp_http_client.h>
// #include <esp_wifi.h>
#include <HardwareSerial.h>
#include <WString.h>
// C:\Users\humti\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.2.0\libraries
// #include <WiFi.h>
// #include <HTTPClient.h>

#include "lib/KIC.h"
// #include <esp_https_ota.h>

// COM9    serial   Serial Port (USB) Arduino Mega or Mega 2560 arduino:avr:mega arduino:avr


/* === esp32 ===
compile script:
arduino-cli compile --fqbn esp32:esp32:esp32wroverkit .\BlackBoardCleaner
*/

/*
upload script:
arduino-cli upload -p COM9 --fqbn esp32:esp32:esp32wroverkit .\BlackBoardCleaner
*/

/* === mega ===
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

// === TEST ===
class KICProtocolTestCase
{
  public:
    static void testKICParser()
    {
      // -- setup --
      bool testPassed = true;

      Serial.begin(9600);
      String str = "KIC:V1;1437;1090010001130;208000900;8;9;/";
      Serial.print("receivedString: ");
      Serial.println(str);

      SPool* spool = KIC::convertToSPool(str);
      const char expectedCmds[] = {'1', '2', '8', '9', '/'};
      const int expectedCounts[] = {3, 2, 0, 0, 0};
      const int expectedHours[][3] = {
        {900, 1000, 1130},
        {800, 900, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
      };

      // -- test --
      Serial.println("=== TEST ===");
      for(unsigned int i = 0; i < spool->count; i++){
        Schedule *schedule = *(spool->schedules + i);
        // Serial.print("Schedule ");
        // Serial.println(i);
        // Serial.print("cmd> ");
        // Serial.println(schedule->cmd);
        // Serial.println("hours@");

        // assert cmd
        if(schedule->cmd != expectedCmds[i]) {
          testPassed = false;
          // Serial.print("ERROR: cmd mismatch at schedule ");
          // Serial.println(i);
        }

        // assert count
        if(schedule->count != expectedCounts[i]){
          testPassed = false;
          // Serial.print("ERROR: hours count mismatch at schedule ");
          // Serial.println(i);
        } else {
          for(unsigned int j = 0; j < schedule->count; j++){
            // Serial.print(schedule->hours[j]);
            // Serial.print(" ");
            if(schedule->hours[j] != expectedHours[i][j]){
              testPassed = false;
              // Serial.print("\nERROR: hour value mismatch at schedule ");
              // Serial.print(i);
              // Serial.print(", hour index ");
              // Serial.println(j);
            }
          }
        }
        // Serial.println("@");
      }
      Serial.println("=== TEST END ===");

      if(testPassed){
        Serial.println("testKICParser: Test passed");
      } else {
        Serial.println("testKICParser: Test failed");
      }

      // -- teardown --
      freeSPool(spool);
    }
};

//ex) KIC:V1;1437;1090010001130;208000900;8;9;/
//ex) KIC:V1;1437;1_0900_1000_1130;2_0800_0900;8;9;/
ScheduleGateway* gateway;
#define KIC_END '/'

static SPool* spool;

void setup() {
  KICProtocolTestCase::testKICParser();
}

void loop() {
}
