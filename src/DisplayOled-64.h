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
    volatile int drawEveryThMs = 200; // in ms 200 = 1000 / 4; fps = 4
    volatile unsigned long drawPrevTime = 0;
    volatile unsigned long drawTimeDx = 0;

    void (*drawCb)(void);
    volatile int lens[2] = {0, 0};

    volatile boolean toClear = false;

private:

    void drawMain(String text, int x, int y, boolean showDec) {
        this->drawTextAlignRight(text, x, y, showDec, 0);
    }

    void drawSecond(String text, int x, int y, boolean showDec) {
        this->drawTextAlignRight(text, x, y, showDec, 1);
    }

    void drawTextAlignRight(String text, int x, int y, boolean showDec, int pos) {
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
        int len = text.length();
        int w = oled.fontWidth();
        int newX = x + (8 * w - (len * w) - len);
        oled.setCursor(newX, y);
        if (this->lens[pos] != len) {
            oled.println(text);
            oled.clearToEOL();
            oled.setCursor(newX, y);
            oled.println(text);
            this->lens[pos] = len;
        } else {
            oled.println(text);
        }
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

    void needClear() {
        this->toClear = true;
    }

    void drawScreen() {
        this->drawLastTime = millis();
        this->drawTimeDx = getTimeDiff(this->drawLastTime, this->drawPrevTime);

        if (this->drawTimeDx > this->drawEveryThMs) {
//            if (this->toClear) {
//                this->clearDisplay();
//                this->toClear = false;
//            }
            oled.setCursor(0, 0);
            this->drawCb();
            this->toClear = false;
            this->fps = 1000 / this->drawTimeDx;

            this->drawPrevTime = this->drawLastTime;
        }
    }

    // right places
    void drawIntMain(long int val) {
        oled.setFont(lcdnums14x24);
        this->drawMain(String(val), 20, 2, false);
    }

    void drawIntSecond(long int val) {
        oled.setFont(lcdnums12x16);
        this->drawSecond(String(val), 35, 6, false);
    }

    void drawDistanceMain(long int val) {
        oled.setFont(lcdnums14x24);
        this->drawMain(String(val), 20, 2, true);
    }

    void drawDistanceSecond(long int val) {
        oled.setFont(lcdnums12x16);
        this->drawSecond(String(val), 35, 6, true);
    }

    void drawTitle(String val) {
        oled.setFont(System5x7);
        oled.setCursor(0, 0);
        oled.println(val);
    }

};