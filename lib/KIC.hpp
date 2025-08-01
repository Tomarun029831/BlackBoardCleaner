#ifndef _KIC_
#define _KIC_
#include "./SPool.hpp"
#include <WString.h>

#define KICVERSION "KIC:V2"
#define KICSEGMENTCHAR ';'
#define KICEND "/"

namespace KIC {
    struct Board{
        unsigned int height;
        unsigned int width;
    };

    struct KICINFO{
        Schedule* timestamp;
        Board board;
        SPool* spool;
    };

    KICINFO* convertToKIC(const String& kic);
}

#endif // !_KIC_

