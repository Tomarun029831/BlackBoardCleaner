#include "../lib/KICCollection.hpp" // CleaningDiagramCollection, WString
#include <HardwareSerial.h>

KICCollection::KICData KICCollection::convertToKIC(const String &kicString) {
  char kicHeader[KICHEADERLENGTH], serverSendTime[SERVERSENDTIMELENGTH],
      boardSize[BOARDSIZELENGTH], *cleanDiagram;
  bool isCorrenctLexed = KICCollection::KICLexer(
      kicString, kicHeader, serverSendTime, boardSize, &cleanDiagram);
  KICData parsedKICData;
  bool isCorrectParsed = KICCollection::KICParser(
      kicHeader, serverSendTime, boardSize, cleanDiagram, parsedKICData);

  return parsedKICData;
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

bool KICCollection::KICLexer(const String &kicString, char *out_kicHeader,
                             char *out_serverTimestamp, char *out_boardSize,
                             char **out_cleanDiagram) {
  if (kicString == NULL)
    return false;
  unsigned int begin = 0, end = 0;

  // TODO: follow DRY-principle

  // extract kicHeader
  end = kicString.indexOf(KICSEGMENTCHAR, begin);
  String kicHeaderStr = kicString.substring(begin, end);
  strncpy(out_kicHeader, kicHeaderStr.c_str(), KICHEADERLENGTH - 1);
  out_kicHeader[KICHEADERLENGTH - 1] = '\0';
  begin = end + 1;
  if (strcmp(out_kicHeader, KICVERSION) != 0)
    return false;

  // check kicEnd
  if (kicString.indexOf(KICEND) == -1)
    return false;

  // extract serverSendTime
  end = kicString.indexOf(KICSEGMENTCHAR, begin);
  String serverSendTimeStr = kicString.substring(begin, end);
  strncpy(out_serverTimestamp, serverSendTimeStr.c_str(),
          SERVERSENDTIMELENGTH - 1);
  out_serverTimestamp[SERVERSENDTIMELENGTH - 1] = '\0';
  begin = end + 1;

  // extract bouardSize
  end = kicString.indexOf(KICSEGMENTCHAR, begin);
  String boardSizeStr = kicString.substring(begin, end);
  strncpy(out_boardSize, boardSizeStr.c_str(), BOARDSIZELENGTH - 1);
  out_boardSize[BOARDSIZELENGTH - 1] = '\0';
  begin = end + 1;

  // extract cleanDiagram
  // end = kicString.indexOf(KICEND, begin);
  // String cleanDiagramStr = kicString.substring(begin, end);
  String cleanDiagramStr = kicString.substring(begin);
  *out_cleanDiagram = strdup(cleanDiagramStr.c_str());

  return true;
}

// KIC:V2;01437;01140334;008001200;20700090011001300;/

// bool KICCollection::KICParser(const char *kicHeader, const char
// *serverSendTime, const char *boardSize, const char *cleanDiagram, KICData&
// out_kicData) {
//     KICData parsedKICData;
//
//     if (strcmp(kicHeader, KICVERSION) != 0) return false;
//     namespace CDC = CleaningDiagramCollection;
//
//     // syntaxCheck serverSendTime
//     if (serverSendTime == nullptr || strlen(serverSendTime) !=
//     (SERVERSENDTIMELENGTH - 1)) return false; KICCollection::ServerTimestamp
//     parsedServerSendTime; parsedServerSendTime.day = serverSendTime[0];
//     parsedServerSendTime.hour_minute = atoi(serverSendTime + 1);
//
//     // syntaxCheck boardSize
//     if (boardSize == nullptr || strlen(boardSize) != (BOARDSIZELENGTH - 1))
//     return false; constexpr unsigned int segmentSize = (BOARDSIZELENGTH - 1)
//     / 2; // segmentSize == 4 -> true char heightSizeStr[segmentSize + 1];
//     strncpy(heightSizeStr, boardSize, segmentSize);
//     heightSizeStr[segmentSize] = '\0';
//     char widthSizeStr[segmentSize + 1];
//     strncpy(widthSizeStr, boardSize + segmentSize, segmentSize);
//     widthSizeStr[segmentSize] = '\0';
//     unsigned int parsedHeightSize = atoi(heightSizeStr); // HACK:
//     unsigned int parsedWidthSize = atoi(widthSizeStr); // HACK:
//
//     // syntaxCheck cleanDiagram
//     if (cleanDiagram == nullptr) return false;
//     CDC::CleaningDiagram parsedDiagram;
//     bool isSuccess = CDC::CleaningDiagramParser(cleanDiagram, parsedDiagram);
//     if(!isSuccess) return false;
//
//     // set & allocate KICData
//     parsedKICData.serverTimestamp = parsedServerSendTime;
//     parsedKICData.board.height = parsedHeightSize;
//     parsedKICData.board.width = parsedWidthSize;
//     parsedKICData.diagram = parsedDiagram;
//
//     out_kicData = parsedKICData;
//     return true;
// }

bool KICCollection::KICParser(const char *kicHeader, const char *serverSendTime,
                              const char *boardSize, const char *cleanDiagram,
                              KICData &out_kicData) {

  KICData parsedKICData;

  // KICヘッダーチェック

  if (strcmp(kicHeader, KICVERSION) != 0) {
    return false;
  }

  namespace CDC = CleaningDiagramCollection;

  // serverSendTimeの構文チェック

  if (serverSendTime == nullptr) {
    return false;
  }

  size_t serverSendTimeLen = strlen(serverSendTime);

  if (serverSendTimeLen != (SERVERSENDTIMELENGTH - 1)) {
    return false;
  }

  KICCollection::ServerTimestamp parsedServerSendTime;
  parsedServerSendTime.day = serverSendTime[0];
  parsedServerSendTime.hour_minute = atoi(serverSendTime + 1);

  // boardSizeの構文チェック

  if (boardSize == nullptr) {
    return false;
  }

  size_t boardSizeLen = strlen(boardSize);

  if (boardSizeLen != (BOARDSIZELENGTH - 1)) {
    return false;
  }

  constexpr unsigned int segmentSize =
      (BOARDSIZELENGTH - 1) / 2; // segmentSize == 4 -> true

  char heightSizeStr[segmentSize + 1];
  strncpy(heightSizeStr, boardSize, segmentSize);
  heightSizeStr[segmentSize] = '\0';

  char widthSizeStr[segmentSize + 1];
  strncpy(widthSizeStr, boardSize + segmentSize, segmentSize);
  widthSizeStr[segmentSize] = '\0';

  unsigned int parsedHeightSize = atoi(heightSizeStr); // HACK:
  unsigned int parsedWidthSize = atoi(widthSizeStr);   // HACK:

  if (cleanDiagram == nullptr) {
    return false;
  }

  CDC::CleaningDiagram parsedDiagram;

  bool isSuccess = CDC::CleaningDiagramParser(cleanDiagram, parsedDiagram);

  if (!isSuccess) {
    return false;
  }

  // KICDataの設定と割り当て
  parsedKICData.serverTimestamp = parsedServerSendTime;
  parsedKICData.board.height = parsedHeightSize;
  parsedKICData.board.width = parsedWidthSize;
  parsedKICData.diagram = parsedDiagram;

  out_kicData = parsedKICData;

  return true;
}
