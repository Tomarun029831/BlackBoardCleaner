#ifndef _KICCollection_
#define _KICCollection_
#include "./CleaningDiagramCollection.hpp"
#include <WString.h>

namespace KICCollection {
    // KIC Syntax
    inline constexpr const char* KICVERSION = "KIC:V3";
    inline constexpr char KICSEGMENTCHAR = ';';
    inline constexpr char KICEND = '/';

    // data-size of KIC Payload
    inline constexpr unsigned int KICHEADERLENGTH = 7; // 6 + 1
    inline constexpr unsigned int SERVERSENDTIMELENGTH = 6; // 5 + 1
    inline constexpr unsigned int BOARDSIZELENGTH = 9; // 4(height) + 4(width) + 1

    struct Board{
        unsigned int height;
        unsigned int width;
    };

    struct ServerTimestamp{
        char day;
        unsigned int hour_minute;
    };

    struct KICData{
        KICCollection::ServerTimestamp serverTimestamp;
        Board board;
        CleaningDiagramCollection::CleaningDiagram diagram;
    };

    KICData convertToKIC(const String& kicString);
    bool KICLexer(const String& kicString, char *out_kicHeader, char *out_serverSendTime, char *out_boardSize, char **out_cleanDiagram);
    bool KICParser(const char *kicHeader, const char *serverSendTime, const char *boardSize, const char *cleanDiagram, KICData& out_kicData);
}

#endif // !_KICCollection_

