#include <Arduino.h>
#include <HardwareSerial.h>
#include <WString.h>

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

struct Schedule {
  public:
    Schedule(unsigned char week, unsigned char hour) : week(week), hour(hour) {}
    unsigned char week : 3;
    unsigned char hour : 5;
};

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
      return new Schedule(0, 0);
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

ScheduleGateway* gateway;

#define KIC_END '/'

class KicParser {
public:
  static void parse(String kic) {
    Serial.print("parse() called with: ");
    Serial.println(kic);

    // 1. 終端文字の確認
    if (!kic.endsWith("/")) {
      Serial.println("Invalid: does not end with '/'");
      return;
    }

    // 2. '/' を削除
    kic.remove(kic.length() - 1);

    // 3. セミコロンで分割
    const int maxParts = 10;
    String parts[maxParts];
    int index = 0;

    while (kic.length() > 0 && index < maxParts) {
      int sepIndex = kic.indexOf(';');
      if (sepIndex == -1) {
        parts[index++] = kic;
        break;
      } else {
        parts[index++] = kic.substring(0, sepIndex);
        kic = kic.substring(sepIndex + 1);
      }
    }

    // 4. デバッグ出力
    for (int i = 0; i < index; ++i) {
      Serial.print("Part ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(parts[i]);
    }

    // 5. スケジュール解析（オプション）
    for (int i = 2; i < index; i++) {
      parseScheduleBlock(parts[i]);
    }
  }

private:
  static void parseScheduleBlock(const String& block) {
    if (block.length() < 5) return; // 曜日 + 最低1時刻
    int day = block.substring(0, 1).toInt();
    Serial.print("曜日: ");
    Serial.println(day);

    String times = block.substring(1);
    for (int i = 0; i + 4 <= times.length(); i += 4) {
      String time = times.substring(i, i + 4);
      Serial.print(" - 時刻: ");
      Serial.println(time);
    }
  }

  KicParser() = delete;
};


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

  KicParser::parse(str);
}

void move(bool toright, bool toup){

}

void move_x(bool toright){

}

void move_y(bool toup){

}
