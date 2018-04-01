// Simple I2C test for ebay 128x64 oled.
// Use smaller faster AvrI2c class in place of Wire.
//
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;

class DisplayOled {
    volatile int FONT_SIZE = 6;
    volatile int drawEveryThMs = 200; // in ms 200 = 1000 / 4; fps = 4
    volatile unsigned long drawPrevTime = 0;
    volatile unsigned long drawTimeDx = 0;

    void (*drawCb)(void);

private:


// defaults
    void drawTextAlignRight(String text, int x, int y, boolean showDec) {
        if (showDec) {
            int textLen = text.length();
            if (textLen == 1) {
                text = "0." + text;
            } else {
                char last = text[textLen - 1];
                text.remove(textLen - 1);
                text = text + '.' + last;
            }
        }

        int w = oled.fontWidth();
        int len = text.length();
        oled.setCursor(x + (8 * w - (len * w) - len), y);
        oled.println(text);
        oled.set1X();
    }

public:
    volatile unsigned long drawLastTime = 0;
    volatile float fps = 0;

    void setupDisplay(String ver, int fps) {

#if RST_PIN >= 0
        oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
        oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

        oled.setFont(System5x7);

        oled.setContrast(255);
        oled.clear();
        oled.println(ver);

        this->drawEveryThMs = round(1000 / fps);
    }

    void onDraw(void(*func)(void)) {
        drawCb = func;
    }

    void clearDisplay() {
        oled.clear();
    }

    void drawScreen() {
        this->drawLastTime = millis();
        this->drawTimeDx = getTimeDiff(this->drawLastTime, this->drawPrevTime);

        if (this->drawTimeDx > this->drawEveryThMs) {
            oled.setCursor(0, 0);
            this->drawCb();
//            this->clearDisplay();
            this->fps = 1000 / this->drawTimeDx;

            this->drawPrevTime = this->drawLastTime;
        }
    }

    // right places
    void drawIntMain(long int val) {
        oled.setFont(lcdnums14x24);
        this->drawTextAlignRight(String(val), 20, 2, false);
    }

    void drawIntSecond(long int val) {
        oled.setFont(lcdnums12x16);
        this->drawTextAlignRight(String(val), 35, 6, false);
    }

    void drawDistanceMain(long int val) {
        oled.setFont(lcdnums14x24);
        this->drawTextAlignRight(String(val), 20, 2, true);
    }

    void drawDistanceSecond(long int val) {
        oled.setFont(lcdnums12x16);
        this->drawTextAlignRight(String(val), 35, 6, true);
    }

    void drawTitle(String val) {
        oled.setFont(System5x7);
        oled.setCursor(0, 0);
        oled.println(val);
    }

};