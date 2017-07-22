#include <Wire.h>
// https://github.com/pololu/vl53l0x-arduino
#include <VL53L0X.h>
// https://github.com/thomasfredericks/Metro-Arduino-Wiring
#include <Metro.h>

VL53L0X sensor;

// - higher accuracy at the cost of lower speed
#define HIGH_ACCURACY

const int LEDPIN = 13;
// PCスクリーンロック回避のために定期的にマウスを動かす
Metro mouseTimer = Metro(540000); // 9 [min]
// 離席かどうかのしきい値
const uint16_t AWAYTHRESHOLD = 650; // [mm]
// マウスを動かす間隔の間で、連続3回以上の在席の検知があったか
boolean wasActive = true;
// 在席の検知が連続何回あったか
int16_t nactives = 0;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Mouse.begin();
    pinMode(LEDPIN, OUTPUT);

    sensor.init();
    sensor.setTimeout(500);
#if defined HIGH_SPEED
    // reduce timing budget to 20 ms (default is about 33 ms)
    sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
    // increase timing budget to 200 ms
    sensor.setMeasurementTimingBudget(200000);
#endif
}

void loop()
{
    if (mouseTimer.check()) {
        if (wasActive) {
            wasActive = false;
            Mouse.move(1, 0, 0);
            Serial.print("M");
        } else {
            Serial.print(".");
        }
    }

    uint16_t mm = sensor.readRangeSingleMillimeters();
    if (!sensor.timeoutOccurred()) {
        boolean isActive = (mm < AWAYTHRESHOLD);
        if (isActive) {
            digitalWrite(LEDPIN, HIGH); // led on
            nactives++;
            if (nactives >= 3) {
                wasActive = true; // 3回以上連続したactiveがあった
            }
        } else {
            digitalWrite(LEDPIN, LOW); // led off
            nactives = 0;
        }
        Serial.print(mm);
        Serial.print(" ");
    }
    delay(1000);
}
