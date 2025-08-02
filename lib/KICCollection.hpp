#ifndef _KICCollection_
#define _KICCollection_
#include "./CleaningDiagramCollection.hpp"
#include <WString.h>

namespace KICCollection {
    // KIC Syntax
    inline constexpr const char KICVERSION[] = "KIC:V2";
    inline constexpr char KICSEGMENTCHAR = ';';
    inline constexpr const char KICEND[] = "/";

    // data-size of KIC Payload
    inline constexpr unsigned int SERVERSENDTIMELENGTH = 5;
    inline constexpr unsigned int BOARDSIZELENGTH = 8; // 4(height) + 4(width)

    struct Board{
        unsigned int height;
        unsigned int width;
    };

    struct KICData{
        CleaningDiagramCollection::DaySchedule serverSendTime;
        Board board;
        CleaningDiagramCollection::CleaningDiagram diagram;
    };

    KICData* convertToKIC(const String& kicString);
    void freeKICData(KICData *data);
    bool KICLexer(const String& kicString, char *kicHeader, char *serverSendTime, char *boardSize, char *cleanDiagram);
    KICData* KICParser(const char *kicHeader, const char *serverSendTime, const char *boardSize, const char *cleanDiagram);
}

#endif // !_KICCollection_

