// DigiMouse.hだと、常に入力ありとみなされて(?)スクリーンセーバが動かなくなる。
// DigiMouse.hの186-191行(time_since_last_report関係)と、140-142行
// (last_send_reportのclear関係)コメントアウトしたDigiMouseQuiet.hを使用
#include "DigiMouseQuiet.h"

const int SENSORPIN = 2;
const int LEDPIN = 1; // LED on Digispark Model A (rev2, rev4)
#define LED_ONACTIVE 1
#define LED_ONAWAY   2
#define LEDON LED_ONAWAY
#if (LEDON == LED_ONACTIVE)
// 最後にセンサの反応がなくなってからこの時間が過ぎたらLED消灯
const uint32_t LEDTIMEOUT = 15000; // [ms]
#endif
uint32_t now;
uint32_t lastActive = 0;
uint32_t lastMouse = 0;

void setup()
{
    DigiMouse.begin();
    pinMode(LEDPIN, OUTPUT);
    pinMode(SENSORPIN, INPUT);
    lastMouse = millis();
}

static void mouseLoop()
{
    // move mouse to avoid screen saver
    const uint32_t PCLOCKMS = 540000; // 9 [min]
    if (now - lastMouse > PCLOCKMS) {
        if (now - lastActive > PCLOCKMS) { // away presence
#if (LEDON == LED_ONAWAY)
            digitalWrite(LEDPIN, HIGH);
#endif
            return; // allow screen saver
        }
        lastMouse = now;
        DigiMouse.moveX(1); // right 1
    }
}

void loop()
{
    now = millis();
    boolean isActive = (digitalRead(SENSORPIN) == HIGH);
    if (isActive) {
        lastActive = now;
#if (LEDON == LED_ONACTIVE)
        digitalWrite(LEDPIN, HIGH);
    } else if (now - lastActive > LEDTIMEOUT) {
        digitalWrite(LEDPIN, LOW);
#endif
    }
    mouseLoop();
    DigiMouse.delay(50);
#if (LEDON == LED_ONAWAY)
    digitalWrite(LEDPIN, LOW);
#endif
}
