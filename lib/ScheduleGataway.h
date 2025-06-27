#ifndef _SCHEDULEGATEWAY_
#define _SCHEDULEGATEWAY_

class ScheduleGateway{
public:
    virtual int available()=0;
    virtual String receiveString()=0;
    virtual void sendString(String str)=0;
};

class SerialGateway : public ScheduleGateway;
class WirelessGateway : public ScheduleGateway;

#endif // !_SCHEDULEGATEWAY_
