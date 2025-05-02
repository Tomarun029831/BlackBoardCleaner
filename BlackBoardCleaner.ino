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
arduino-cli compile --fqbn arduino:avr:mega .\BlackBoardCleaner && arduino-cli upload -p COM9 --fqbn arduino:avr:mega .\BlackBoardCleaner
*/

//
struct Schedule {
  public:
    Schedule(unsigned char week, unsigned char hour) : week(week), hour(hour) {}
    unsigned char week : 3;
    unsigned char hour : 5;
};

class ScheduleGateway{
  public:
    virtual Schedule getSchedule() = 0;

  protected:
    virtual unsigned char getWeek() = 0;
    virtual unsigned char getHour() = 0;
};

class SerialGateway : ScheduleGateway{
  public:
    Schedule getSchedule() override{
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

class WirelessGateway : ScheduleGateway{
  public:
    Schedule getSchedule() override{
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

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() <= 0) return;

  String str = Serial.readStringUntil(4); // ^D = ASCII 4
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("receiption: ");
  Serial.println(str);
}

void move(bool toright, bool toup){

}

void move_x(bool toright){

}

void move_y(bool toup){

}
