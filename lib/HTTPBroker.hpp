#ifndef _HTTPBROKER_
#define _HTTPBROKER_
#include <WString.h>

namespace HTTPBroker {
    void setup();
    bool available();
    String receiveString();
}

#endif // !_HTTPBROKER_
