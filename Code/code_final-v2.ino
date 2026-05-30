#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <U8g2lib.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_TCS34725 tcs = Adafruit_TCS34725(
    TCS34725_INTEGRATIONTIME_50MS,
    TCS34725_GAIN_1X
);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const char* getColorName(uint8_t r, uint8_t g, uint8_t b, uint16_t c)
{
    // White
    if (abs((int)r - (int)g) < 15 &&
        abs((int)g - (int)b) < 15 &&
        abs((int)r - (int)b) < 15 &&
        c > 800)
    {
        return "White";
    }

    // Yellow
    if (r > 110 &&
        g > 110 &&
        b < 60)
    {
        return "Yellow";
    }

    // Orange
    if (r > 120 &&
        g > 60 &&
        g < 120 &&
        b < 50)
    {
        return "Orange";
    }

    // Brown
    if (r > 80 &&
        g > 40 &&
        g < 130 &&
        b < 80 &&
        r >= g - 20)
    {
        return "Brown";
    }

    // Tan
    if (r > 110 &&
        g > 90 &&
        b > 50 &&
        r >= g &&
        g > b)
    {
        return "Tan";
    }

    // Purple
    if (r > 75 &&
        b > 75 &&
        g < 80)
    {
        return "Purple";
    }

    // Pink
    if (r > 110 &&
        b > 80 &&
        g < 100)
    {
        return "Pink";
    }

    // Red
    if (r > g + 15 &&
        r > b + 15)
    {
        return "Red";
    }

    // Green
    if (g > r + 25 &&
        g > b + 25)
    {
        return "Green";
    }

    // Blue
    if (b > r + 15 &&
        b > g + 15)
    {
        return "Blue";
    }

    return "";
}

void setup()
{
    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN);

    u8g2.begin();
    u8g2.setFont(u8g2_font_6x12_tf);

    if (!tcs.begin())
    {
        u8g2.clearBuffer();
        u8g2.drawStr(10, 30, "TCS FAIL");
        u8g2.sendBuffer();

        while (1);
    }

    Serial.println("Colour Sensor Ready");
}

void loop()
{
    uint16_t r, g, b, c;

    tcs.getRawData(&r, &g, &b, &c);

    const char* colourName = "";

    // Black detection
    if (c < 120)
    {
        colourName = "Black";

        u8g2.clearBuffer();
        u8g2.drawStr(10, 12, "COLOUR SCANNER");
        u8g2.drawStr(10, 30, "R:0 G:0 B:0");
        u8g2.drawStr(10, 50, colourName);
        u8g2.sendBuffer();

        delay(500);
        return;
    }

    uint32_t total_rgb = r + g + b;

    if (total_rgb == 0)
        total_rgb = 1;

    uint8_t red   = ((uint32_t)r * 255) / total_rgb;
    uint8_t green = ((uint32_t)g * 255) / total_rgb;
    uint8_t blue  = ((uint32_t)b * 255) / total_rgb;

    // Small green boost
    green = min(255, (int)(green * 1.10));

    Serial.print("R:");
    Serial.print(red);
    Serial.print(" G:");
    Serial.print(green);
    Serial.print(" B:");
    Serial.print(blue);
    Serial.print(" C:");
    Serial.println(c);

    colourName = getColorName(red, green, blue, c);

    char line[32];
    snprintf(line, sizeof(line), "R:%d G:%d B:%d", red, green, blue);

    u8g2.clearBuffer();

    u8g2.drawStr(10, 12, "COLOUR SCANNER");
    u8g2.drawStr(10, 30, line);

    if (strlen(colourName) > 0)
    {
        u8g2.drawStr(10, 50, colourName);
    }

    u8g2.sendBuffer();

    delay(500);
}