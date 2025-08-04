#include "../lib/KICCollection.hpp" // CleaningDiagramCollection, WString

KICCollection::KICData *KICCollection::convertToKIC(const String& kicString) {
    char *kicHeader, *serverSendTime, *boardSize, *cleanDiagram;
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

bool KICCollection::KICLexer(const String& kicString, char *kicHeader, char *serverSendTime, char *boardSize, char **cleanDiagram) {
    if (kicString == NULL) return false;
    unsigned int begin = 0, end = 0;

    // TODO: follow DRY-principle

    // extract kicHeader
    end = kicString.indexOf(KICSEGMENTCHAR, begin);
    String kicHeaderStr = kicString.substring(begin, end);
    strncpy(kicHeader, kicHeaderStr.c_str(), KICHEADERLENGTH - 1)
    kicHeader[KICHEADERLENGTH - 1] = '\0';
    begin = end + 1;
    if (strcmp(kicHeader, KICVERSION) != 0) return false;

    // check kicEnd
    if (kicString.indexOf(KICEND) == -1) return false;

    // extract serverSendTime
    end = kicString.indexOf(KICSEGMENTCHAR, begin);
    String serverSendTimeStr = kicString.substring(begin, end);
    strncpy(serverSendTime, serverSendTimeStr.c_str(), SERVERSENDTIMELENGTH - 1);
    serverSendTime[SERVERSENDTIMELENGTH - 1] = '\0';
    begin = end + 1;

    // extract boardSize
    end = kicString.indexOf(KICSEGMENTCHAR, begin);
    String boardSizeStr = kicString.substring(begin, end);
    strncpy(boardSize, boardSizeStr.c_str(), BOARDSIZELENGTH - 1);
    boardSize[BOARDSIZELENGTH  - 1] = '\0';
    begin = end + 1;

    // extract cleanDiagram
    // end = kicString.indexOf(KICEND, begin);
    // String cleanDiagramStr = kicString.substring(begin, end);
    String cleanDiagramStr = kicString.substring(begin);
    *cleanDiagram = strdup(cleanDiagramStr.c_str());

    return true;
}

// KIC:V2;01437;01140334;008001200;20700090011001300;/

KICCollection::KICData *KICCollection::KICParser(const char *kicHeader, const char *serverSendTime, const char *boardSize, const char *cleanDiagram) {
    if (strcmp(kicHeader, KICVERSION) != 0) return nullptr;
    namespace CDC = CleaningDiagramCollection;

    // syntaxCheck serverSendTime
    if (serverSendTime == nullptr && strlen(serverSendTime) != SERVERSENDTIMELENGTH) return nullptr;
    String temp(serverSendTime); // HACK:
    CDC::DaySchedule parsedServerSendTime = CDC::ScheduleParser(temp);
    if(parsedServerSendTime.length == 0) return nullptr;

    // syntaxCheck boardSize
    if (boardSize == nullptr && strlen(boardSize) != BOARDSIZELENGTH - 1) return nullptr;
    unsigned int segmentSize = (BOARDSIZELENGTH - 1) / 2;
    char *heightSizeStr = strndup(boardSize, segmentSize);
    char *widthSizeStr = strndup(boardSize + segmentSize, segmentSize);
    unsigned int parsedHeightSize = atoi(heightSizeStr); // HACK:
    unsigned int parsedWidthSize = atoi(widthSizeStr); // HACK:

    // syntaxCheck cleanDiagram
    if (cleanDiagram == nullptr) return nullptr;
    CDC::CleaningDiagram parsedDiagram = {nullptr, 0};
    String temp1(cleanDiagram); // HACK:
    bool isSuccess = CDC::CleaningDiagramParser(temp1, &parsedDiagram);
    if(!isSuccess) return nullptr;

    // set & allocate KICData
    KICData *parsedKICData = (KICData *)calloc(1, sizeof(KICData));
    parsedKICData->serverSendTime = parsedServerSendTime;
    parsedKICData->board.height = parsedHeightSize;
    parsedKICData->board.width = parsedWidthSize;
    parsedKICData->diagram = parsedDiagram;

    return parsedKICData;
}
