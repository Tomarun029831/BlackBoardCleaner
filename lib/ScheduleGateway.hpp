#ifndef _SCHEDULEGATEWAY_
#define _SCHEDULEGATEWAY_

#include <WString.h>

class ScheduleGateway{
public:
    virtual int setup() = 0; virtual int available()=0;
    virtual String receiveString()=0;
    virtual void sendString(String str)=0;
};

class SerialGateway : public ScheduleGateway {
public:
    int setup() override;
    int available() override;
    String receiveString() override;
    void sendString(String str) override;
};

class WirelessGateway : public ScheduleGateway {
public:
    int setup() override;
    int available() override;
    String receiveString() override;
    void sendString(String str) override;
};


#endif // !_SCHEDULEGATEWAY_
