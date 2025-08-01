#ifndef _SCHEDULEGATEWAY_
#define _SCHEDULEGATEWAY_

#include <WString.h>

class ScheduleGateway{
public:
    virtual bool setup() = 0;
    virtual bool available()=0;
    virtual String receiveString()=0;
    virtual void sendString(String str)=0;
};

class SerialGateway : public ScheduleGateway {
public:
    bool setup() override;
    bool available() override;
    String receiveString() override;
    void sendString(String str) override;
};

class WirelessGateway : public ScheduleGateway {
public:
    bool setup() override;
    bool available() override;
    String receiveString() override;
    void sendString(String str) override;
};

#endif // !_SCHEDULEGATEWAY_
