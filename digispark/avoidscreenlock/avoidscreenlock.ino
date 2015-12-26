#include <DigiMouse.h>

const int SENSORPIN = 2;
const int LEDPIN = 1; // LED on Digispark Model A (rev2, rev4)
// 最後にセンサの反応がなくなってからこの時間が過ぎたらLED消灯
const uint32_t LEDTIMEOUT = 15000; // [ms]
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
        //digitalWrite(LEDPIN, HIGH);
        lastActive = now;
    } else if (now - lastActive > LEDTIMEOUT) {
        digitalWrite(LEDPIN, LOW);
    }
    mouseLoop();
    // XXX: 入力ありとみなされて(?)スクリーンセーバが動かなくなる。
    // が、(DigiMouse.update()も)呼ばなかったら、moveX()しても効かなくなる
    //DigiMouse.delay(50);
    delay(50);
}
