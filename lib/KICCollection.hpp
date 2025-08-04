#ifndef _KICCollection_
#define _KICCollection_
#include "./CleaningDiagramCollection.hpp"
#include <WString.h>

namespace KICCollection {
    // KIC Syntax
    inline constexpr const char KICVERSION[] = "KIC:V2";
    inline constexpr char KICSEGMENTCHAR = ';';
    inline constexpr char KICEND = '/';

    // data-size of KIC Payload
    inline constexpr unsigned int KICHEADERLENGTH = 7; // 6 + 1
    inline constexpr unsigned int SERVERSENDTIMELENGTH = 6; // 5 + 1
    inline constexpr unsigned int BOARDSIZELENGTH = 9; // 4(height) + 4(width) + 1
    inline constexpr unsigned int DAYSCHEDULELENGTH = 4; // 4

    struct Board{
        unsigned int height;
        unsigned int width;
    };

    struct ServerTimestamp{
        char day;
        unsigned int hour_minute;
    };

    struct KICData{
        bool isNull;
        KICCollection::ServerTimestamp serverTimestamp;
        Board board;
        CleaningDiagramCollection::CleaningDiagram diagram;
    };

    KICData convertToKIC(const String& kicString);
    void freeKICData(KICData *data);

    bool KICLexer(const String& kicString, char *kicHeader, char *serverSendTime, char *boardSize, char **cleanDiagram);
    KICData KICParser(const char *kicHeader, const char *serverSendTime, const char *boardSize, const char *cleanDiagram);
}

#endif // !_KICCollection_

