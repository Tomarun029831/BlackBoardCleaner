// === Arduino ===
#include <HardwareSerial.h>

// === modules ===
#include <kic_notation.h>

// === my libs ===
#include "./lib/WheelController.hpp"
#include "./lib/HTTPBroker.hpp"

/*
let port = "COM10"; let fqbn = "esp32:esp32:esp32";
arduino-cli compile --fqbn $fqbn ~/Documents/BlackBoardCleaner/; arduino-cli upload -p $port --fqbn $fqbn ~/Documents/BlackBoardCleaner/; plink -serial $port -sercfg 115200,8,n,1,N
*/

// === Global States ===
bool isOnceCleaned;
String receiveString = "";
static constexpr int machineWidth = 22;   // cm
static constexpr int machineHeight = 22;  // cm
static bool rightMoveToClean = true;
static bool isPositionedUpper = true;

KIC_Timestamp machineInternalTimestamp = TIMESTAMP(0, 0);
constexpr unsigned long one_minute_mills = 60000;
unsigned long mills_on_called;

// === Functions ===
static void AutoClean(const BoardSize boardSize) {
  const int heightToMove = boardSize.height_cm - machineHeight;
  int leftWidthToMove = boardSize.width_cm - machineWidth;

  if (heightToMove <= 0 || leftWidthToMove <= 0) {
    return;
  }

  constexpr int widthToMove = 9;  // cm
  constexpr int forwardDistanceToSide = 10;
  constexpr int backwardDistanceToSide = 10;
  constexpr int forwardDistanceToFixPosition = 10;
  constexpr int backwardDistanceToFixPosition = 10;

  while (true) {
    // 1. Vertical Clean
    if (isPositionedUpper) {
      WheelController::forward(heightToMove);
      isPositionedUpper = false;
    } else {
      WheelController::backward(heightToMove);
      isPositionedUpper = true;
    }

    if (leftWidthToMove <= widthToMove) break;
    // 2. Horizontal Shift
    if (rightMoveToClean) {
      if (!isPositionedUpper) {  // 下部からの右シフト
        WheelController::rightRotate();
        WheelController::backward(backwardDistanceToSide);
        WheelController::leftRotate();
        WheelController::forward(forwardDistanceToFixPosition);

      } else {  // 上部からの右シフト
        WheelController::leftRotate();
        WheelController::forward(forwardDistanceToSide);
        WheelController::rightRotate();
        WheelController::backward(backwardDistanceToFixPosition);
      }
    } else {
      if (!isPositionedUpper) {  // 下部からの左シフト
        WheelController::leftRotate();
        WheelController::backward(backwardDistanceToSide);
        WheelController::rightRotate();
        WheelController::forward(forwardDistanceToFixPosition);

      } else {  // 上部からの左シフト
        WheelController::rightRotate();
        WheelController::forward(forwardDistanceToSide);
        WheelController::leftRotate();
        WheelController::backward(backwardDistanceToFixPosition);
      }
    }

    leftWidthToMove -= widthToMove;
  }

  rightMoveToClean = !rightMoveToClean;
  WheelController::stop();
}

void KIC_Timestamp_Printf(KIC_Timestamp ts) {
  if (ts.segments.is_invalid) {
    printf("[INVALID]\n");
    return;
  }

  uint32_t h = ts.segments.hour_min / 100;
  uint32_t m = ts.segments.hour_min % 100;

  printf("[0x%08X] Day %u, %02u:%02u:%02u.%03u %s\n",
         ts.raw,
         ts.segments.day,
         h,
         m,
         ts.segments.second,
         ts.segments.millisecond,
         ts.segments.is_PM ? "PM" : "AM");
}

#define DEBUG_MODE 0
#define WIFI_MODE 1

void IRAM_ATTR onTimerUpdate(void *arg) {
  KIC_Timestamp_AddMs(&machineInternalTimestamp, 100);
}

void setup() {
  WheelController::setupPinMode();
  WheelController::stop();

  const esp_timer_create_args_t timer_args = {
    .callback = &onTimerUpdate,
    .name = "clock_update"
  };
  esp_timer_handle_t timer_handle;
  esp_timer_create(&timer_args, &timer_handle);
  esp_timer_start_periodic(timer_handle, 100000);
#if DEBUG_MODE
  Serial.begin(115200);
  Serial.println("[DEBUG_MODE]");
  AutoClean(BOARDSIZE(50, 70));
#endif

#if WIFI_MODE
  Serial.begin(115200);
  Serial.println("[WIFI_MODE]");
  HTTPBroker::setup();
  receiveString = HTTPBroker::receiveString();
  Serial.print("receiveString: ");
  Serial.println(receiveString);
  if (check_kic_syntax(receiveString.c_str()) == KIC_SYNTAX_ERROR) ESP.restart();
  machineInternalTimestamp = get_kic_timestamp(receiveString.c_str());
  isOnceCleaned = false;
#endif
}

#if DEBUG_MODE
void loop() {
  KIC_Timestamp_Printf(machineInternalTimestamp);
}
#else
void loop() {
  static uint32_t last_min = 99;
  uint32_t current_min = machineInternalTimestamp.segments.hour_min % 100;

  if (machineInternalTimestamp.segments.day == 6) {
    String newData = HTTPBroker::receiveString();
    if (check_kic_syntax(newData.c_str()) == KIC_SYNTAX_CORRECT) {
      receiveString = newData;
      machineInternalTimestamp = get_kic_timestamp(receiveString.c_str());
    }
  }

  if (current_min != last_min) {
    isOnceCleaned = false;
    last_min = current_min;
  }

  KIC_SchedulePtr daySchedule = find_kic_schedule(receiveString.c_str(), machineInternalTimestamp.segments.day);
  if (daySchedule != KIC_SCHEDULE_NOT_FOUND && !isOnceCleaned) {
    for (size_t idx = 0;; idx++) {
      KIC_Timestamp scheduledTime = find_kic_time_in_schedule(daySchedule, idx);

      if (scheduledTime.segments.is_invalid) break;

      if (machineInternalTimestamp.segments.hour_min == scheduledTime.segments.hour_min && machineInternalTimestamp.segments.is_PM == scheduledTime.segments.is_PM) {

        isOnceCleaned = true;

        AutoClean(get_kic_boardsize(receiveString.c_str()));
        break;
      }
    }
  }

  vTaskDelay(pdMS_TO_TICKS(100));
}
#endif
