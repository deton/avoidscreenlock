#include <Wire.h>
#include <Mouse.h>
// https://github.com/pololu/vl53l0x-arduino
#include <VL53L0X.h>
// https://github.com/SofaPirate/Chrono
#include <Chrono.h>
// http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html
#include "I2C_ClearBus.h"

#define DEBUG 1

VL53L0X sensor;
boolean badSensor = false;

const int LEDPIN = 13;
// PCスクリーンロック回避のために定期的にマウスを動かす
Chrono mouseTimer(Chrono::SECONDS);
// 離席かどうかのしきい値
const uint16_t AWAYTHRESHOLD = 600; // [mm]
// マウスを動かす間隔の間で、連続3回以上の在席の検知があったか
boolean wasActive = true;
// 在席の検知が連続何回あったか
int16_t nactives = 0;

bool clear_i2c()
{
    int rtn = I2C_ClearBus();
    if (rtn != 0) {
#if DEBUG
        Serial.println(F("I2C bus error. Could not clear"));
        if (rtn == 1) {
            Serial.println(F("SCL clock line held low"));
        } else if (rtn == 2) {
            Serial.println(F("SCL clock line held low by slave clock stretch"));
        } else if (rtn == 3) {
            Serial.println(F("SDA data line held low"));
        }
#endif
        return false;
    } else { // bus clear
        // re-enable Wire
        // now can start Wire Arduino master
        Wire.begin();
    }
    return true;
}

bool reset_sensor()
{
    if (!clear_i2c()) {
        return false;
    }
    sensor.setTimeout(500);
    if (!sensor.init()) {
#if DEBUG
        Serial.println("Failed to detect and initialize sensor!");
#endif
        return false;
    }
    sensor.startContinuous(1000);
    return true;
}

void setup()
{
#if DEBUG
    Serial.begin(9600);
#endif
    Mouse.begin();
    pinMode(LEDPIN, OUTPUT);
    if (!reset_sensor()) {
        badSensor = true;
        return;
    }
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

    uint16_t mm = sensor.readRangeContinuousMillimeters();
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
        if (!reset_sensor()) {
            badSensor = true;
        }
    }
}
