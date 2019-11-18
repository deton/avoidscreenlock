#include <Wire.h>
#include <Mouse.h>
// https://github.com/pololu/vl53l0x-arduino
#include <VL53L0X.h>
// https://github.com/SofaPirate/Chrono
#include <Chrono.h>

#define DEBUG 1

VL53L0X sensor;
boolean badSensor = false;

// - higher accuracy at the cost of lower speed
#define HIGH_ACCURACY

const int LEDPIN = 13;
// PCスクリーンロック回避のために定期的にマウスを動かす
Chrono mouseTimer(Chrono::SECONDS);
// 離席かどうかのしきい値
const uint16_t AWAYTHRESHOLD = 600; // [mm]
// マウスを動かす間隔の間で、連続3回以上の在席の検知があったか
boolean wasActive = true;
// 在席の検知が連続何回あったか
int16_t nactives = 0;

void setup()
{
#if DEBUG
    Serial.begin(9600);
#endif
    // Teensy LC SDA1/SCL1
    //Wire.setSDA(23);
    //Wire.setSCL(22);
    Wire.begin();
    Mouse.begin();
    pinMode(LEDPIN, OUTPUT);

    sensor.setTimeout(500);
    if (!sensor.init()) {
#if DEBUG
        Serial.println("Failed to detect and initialize sensor!");
#endif
        badSensor = true;
        return;
    }
#if defined HIGH_SPEED
    // reduce timing budget to 20 ms (default is about 33 ms)
    sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
    // increase timing budget to 200 ms
    sensor.setMeasurementTimingBudget(200000);
#endif
#if DEBUG
    Serial.println("starting");
#endif
}

void loop()
{
    delay(1000);

    if (badSensor) {
        digitalWrite(LEDPIN, HIGH); // led on
        delay(2000);
        digitalWrite(LEDPIN, LOW); // led off
        return;
    }

    if (mouseTimer.hasPassed(9 * 60, true)) { // 9 [min]
        if (wasActive) {
            wasActive = false;
            Mouse.move(1, 0, 0);
#if DEBUG
            Serial.print("M");
#endif
        } else {
#if DEBUG
            Serial.print(".");
#endif
        }
#if DEBUG > 1
        Serial.println();
#endif
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
#if DEBUG > 1
        Serial.print(mm);
        Serial.print(" ");
#endif
    } else {
#if DEBUG
        Serial.print("T");
#endif
    }
}
