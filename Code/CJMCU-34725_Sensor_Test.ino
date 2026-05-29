#include "config.h"
#include "sensor.h"
#include "color_match.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("=== ESP32 NodeMCU + CJMCU-34725 ===");

    Wire.begin(SDA_PIN, SCL_PIN);

    if (!sensor_init()) {
        Serial.println("ERROR: Sensor not found. Check wiring.");
        while (1) delay(10);
    }

    Serial.println("Sensor ready!\n");
}

void loop() {
    uint8_t r, g, b;
    sensor_read(r, g, b);

    Serial.print("RGB(");
    Serial.print(r); Serial.print(", ");
    Serial.print(g); Serial.print(", ");
    Serial.print(b); Serial.print(")  -->  ");
    Serial.println(identify_color(r, g, b));

    delay(READING_INTERVAL_MS);
}
