#ifndef _KICCollection_
#define _KICCollection_
#include "./DiagramCollection.hpp"
#include <WString.h>

namespace KICCollection {
    constexpr char *KICVERSION = "KIC:V2";
    constexpr char KICSEGMENTCHAR = ';';
    constexpr char *KICEND = "/";

    struct Board{
        unsigned int height;
        unsigned int width;
    };

    struct KICData{
        DaySchedule* serverSendTime;
        Board board;
        CleaningDiagram diagram;
    };

    KICData* convertToKIC(const String& kicString);
    void KICLexer(const String& kicString, char *kicHeader, char *serverSendTime, char *boardSize, char *cleanDiagram);
    KICData* KICParser(const char *kicHeader, const char *serverSendTime, const char *boardSize, const char *cleanDiagram);
}

#endif // !_KICCollection_

