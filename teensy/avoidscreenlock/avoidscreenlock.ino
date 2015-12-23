const int SENSORPIN = 23;
const int LEDPIN = 13;
// 最後にセンサの反応がなくなってからこの時間が過ぎたらLED消灯
const uint32_t LEDTIMEOUT = 15000; // [ms]
uint32_t now;
uint32_t lastActive = 0;
uint32_t lastMouse = 0;

void setup()
{
    Serial.begin(9600);
    Mouse.begin();
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
            Serial.print("A");
            return; // allow screen saver
        }
        lastMouse = now;
        Mouse.move(1, 0, 0);
        //Mouse.end();
        Serial.print("M");
    }
}

void loop()
{
    now = millis();
    boolean isActive = (digitalRead(SENSORPIN) == HIGH);
    if (isActive) {
        digitalWrite(LEDPIN, HIGH);
        lastActive = now;
    } else if (now - lastActive > LEDTIMEOUT) {
        digitalWrite(LEDPIN, LOW);
    }
    mouseLoop();
    delay(10);
}
