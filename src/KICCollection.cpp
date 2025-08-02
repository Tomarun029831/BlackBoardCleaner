#include "../lib/KICCollection.hpp" // CleaningDiagramCollection, WString

KICCollection::KICData *KICCollection::convertToKIC(const String& kicString) {
    char *kicHeader, serverSendTime, boardSize, cleanDiagram;
    KICCollection::KICLexer(kicString, kicHeader, serverSendTime, boardSize, cleanDiagram);
    KICData *kicData = KICCollection::KICParser(kicHeader, serverSendTime, boardSize, cleanDiagram);
    if (kicData == nullptr) return nullptr;
    return kicData;
}

void KICCollection::freeKICData(KICData *data) {
    free(data->serverSendTime.hours);
    free(data->diagram.schedules);
    free(data);
}

/*
https://docs.arduino.cc/built-in-examples/strings/StringSubstring/
substring(begin, end);

String stringOne = "Content-Type: text/html";
// you can also look for a substring in the middle of a string:
if (stringOne.substring(14,18) == "text") {}

https://docs.arduino.cc/built-in-examples/strings/StringIndexOf/

String stringOne = "<HTML><HEAD><BODY>";
int firstClosingBracket = stringOne.indexOf('>'); // 5
*/

bool KICCollection::KICLexer(const String& kicString, char *kicHeader, char *serverSendTime, char *boardSize, char *cleanDiagram) {
    if (kicString == NULL) return false;
    unsigned int begin = 0, end = 0;

    // TODO: follow DRY-principle

    // extract kicHeader
    end = kicString.indexOf(KICSEGMENTCHAR, begin);
    String kicHeaderStr = kicString.substring(begin, end);
    kicHeader = strdup(kicHeaderStr.c_str());
    begin = end + 1;
    if (strcmp(kicHeader, KICVERSION) != 0) return false;

    // check kicEnd
    if (kicString.indexOf(kicEnd) == -1) return false;

    // extract serverSendTime
    end = kicString.indexOf(KICSEGMENTCHAR, begin);
    String serverSendTimeStr = kicString.substring(begin, end);
    serverSendTime = strdup(serverSendTimeStr.c_str());
    begin = end + 1;

    // extract boardSize
    end = kicString.indexOf(KICSEGMENTCHAR, begin);
    String boardSizeStr = kicString.substring(begin, end);
    boardSize = strdup(boardSizeStr.c_str());
    begin = end + 1;

    // extract cleanDiagram
    end = kicString.indexOf(KICEND, begin);
    String cleanDiagramStr = kicString.substring(begin, end);
    cleanDiagram = strdup(cleanDiagramStr.c_str());

    return true;
}

// KIC:V2;01437;01140334;008001200;20700090011001300;/

KICCollection::KICData *KICCollection::KICParser(const char *kicHeader, const char *serverSendTime, const char *boardSize, const char *cleanDiagram) {
    if (kicString == nullptr && strcmp(kicHeader, KICVERSION) != 0) return nullptr;
    namespace CDC = CleaningDiagramCollection;

    // syntaxCheck serverSendTime
    if (serverSendTime == nullptr && strlen(serverSendTime) != SERVERSENDTIMELENGTH) return nullptr;
    CDC::DaySchedule parsedServerSendTime = CDC::ScheduleParser(serverSendTime);
    if(parsedServerSendTime == nullptr) return nullptr;

    // syntaxCheck boardSize
    if (boardSize == nullptr && strlen(boardSize) != BOARDSIZELENGTH) return nullptr;
    unsigned int segmentSize = BOARDSIZELENGTH / 2;
    char *heightSizeStr = strndup(boardSize, segmentSize);
    char *widthSizeStr = strndup(boardSize + segmentSize, segmentSize);
    unsigned int parsedHeightSize = atoi(heightSizeStr); // HACK:
    unsigned int parsedWidthSize = atoi(widthSizeStr); // HACK:

    // syntaxCheck cleanDiagram
    if (cleanDiagram == nullptr) return nullptr;
    CDC::cleanDiagram parsedDiagram = nullptr;
    bool isSuccess = CDC::CleaningDiagramParser(cleanDiagram, parsedDiagram);
    if(!isSuccess) return nullptr;

    // set & allocate KICData
    KICData *parsedKICData = calloc(1, sizeof(KICData));
    parsedKICData->serverSendTime = parsedServerSendTime;
    parsedKICData->board.height = parsedHeightSize;
    parsedKICData->board.width = parsedWidthSize;
    parsedKICData->parsedDiagram = parsedDiagram;

    return parsedKICData;
}
