#include "../lib/WheelController.hpp"
#include "../lib/Timestamp.hpp"
#include <driver/ledc.h>
#include <driver/gpio.h>

extern Timestamp machineInternalTimestamp;
extern void delayWithoutCpuStop(unsigned int ms, Timestamp &ts);

namespace WheelController {

// =====================
// Motor pin definitions
// =====================
static constexpr gpio_num_t LEFT_MOTOR_PIN0 = (gpio_num_t)33;
static constexpr gpio_num_t LEFT_MOTOR_PIN1 = (gpio_num_t)32;
static constexpr gpio_num_t RIGHT_MOTOR_PIN0 = (gpio_num_t)26;
static constexpr gpio_num_t RIGHT_MOTOR_PIN1 = (gpio_num_t)25;

// =====================
// PWM (LEDC) Config
// =====================
static constexpr ledc_mode_t LEDC_MODE = LEDC_LOW_SPEED_MODE;
static constexpr ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
// v2.0.11では ledc_timer_bit_t を使用します
static constexpr ledc_timer_bit_t LEDC_RESOLUTION = LEDC_TIMER_10_BIT; 
static constexpr uint32_t LEDC_FREQ = 5000;

static constexpr uint32_t DRIVE_DUTY = 512; // 50% power
static constexpr uint32_t STOP_DUTY = 1023; // High for brake

// =====================
// Time estimation functions (定義を前方に移動)
// =====================
uint32_t estimateTime_forward(unsigned int distance_cm) {
    if (distance_cm == 0) return 0;
    float coefficient = 139.11f;
    float intercept = -81.33f;
    float time_ms = (coefficient * (distance_cm + 1)) + intercept;
    return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

uint32_t estimateTime_backward(unsigned int distance_cm) {
    if (distance_cm == 0) return 0;
    float coefficient = 206.27f;
    float intercept = 160.85f;
    float time_ms = (coefficient * distance_cm) + intercept;
    return (time_ms < 0) ? 0 : static_cast<uint32_t>(time_ms);
}

// =====================
// PWM Control Helpers
// =====================
static void setDuty(ledc_channel_t ch, uint32_t duty) {
    ledc_set_duty(LEDC_MODE, ch, duty);
    ledc_update_duty(LEDC_MODE, ch);
}

static void allLow() {
    setDuty(LEDC_CHANNEL_0, 0);
    setDuty(LEDC_CHANNEL_1, 0);
    setDuty(LEDC_CHANNEL_2, 0);
    setDuty(LEDC_CHANNEL_3, 0);
}

// =====================
// GPIO & PWM Setup
// =====================
void setupPinMode() {
    ledc_timer_config_t timer_conf = {};
    timer_conf.speed_mode = LEDC_MODE;
    timer_conf.duty_resolution = LEDC_RESOLUTION;
    timer_conf.timer_num = LEDC_TIMER;
    timer_conf.freq_hz = LEDC_FREQ;
    timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_conf);

    gpio_num_t pins[] = {LEFT_MOTOR_PIN0, LEFT_MOTOR_PIN1, RIGHT_MOTOR_PIN0, RIGHT_MOTOR_PIN1};
    ledc_channel_t chans[] = {LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3};

    for (int i = 0; i < 4; i++) {
        ledc_channel_config_t ch_conf = {};
        ch_conf.gpio_num = pins[i];
        ch_conf.speed_mode = LEDC_MODE;
        ch_conf.channel = chans[i];
        ch_conf.intr_type = LEDC_INTR_DISABLE;
        ch_conf.timer_sel = LEDC_TIMER;
        ch_conf.duty = 0;
        ch_conf.hpoint = 0;
        ledc_channel_config(&ch_conf);
    }
}

// =====================
// Motion APIs
// =====================
void stop() {
    setDuty(LEDC_CHANNEL_0, STOP_DUTY);
    setDuty(LEDC_CHANNEL_1, STOP_DUTY);
    setDuty(LEDC_CHANNEL_2, STOP_DUTY);
    setDuty(LEDC_CHANNEL_3, STOP_DUTY);
}

void forward(unsigned int cm) {
    if (cm == 0) return;
    allLow();
    setDuty(LEDC_CHANNEL_0, DRIVE_DUTY); // Left Forward
    setDuty(LEDC_CHANNEL_2, DRIVE_DUTY); // Right Forward

    uint32_t delay_ms = estimateTime_forward(cm);
    delayWithoutCpuStop(delay_ms, machineInternalTimestamp);
    stop();
}

void backward(unsigned int cm) {
    if (cm == 0) return;
    allLow();
    setDuty(LEDC_CHANNEL_1, DRIVE_DUTY); // Left Backward
    setDuty(LEDC_CHANNEL_3, DRIVE_DUTY); // Right Backward

    uint32_t delay_ms = estimateTime_backward(cm);
    delayWithoutCpuStop(delay_ms, machineInternalTimestamp);
    stop();
}

void rightRotate(unsigned int degree) {
    allLow();
    setDuty(LEDC_CHANNEL_0, DRIVE_DUTY);
    delayWithoutCpuStop(900, machineInternalTimestamp); // 固定90度
    stop();
}

void leftRotate(unsigned int degree) {
    allLow();
    setDuty(LEDC_CHANNEL_2, DRIVE_DUTY);
    delayWithoutCpuStop(900, machineInternalTimestamp); // 固定90度
    stop();
}

} // namespace WheelController
