#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

class DisplayOled {
    volatile int FONT_SIZE = 6;
    volatile int drawEveryThMs = 200; // in ms
    volatile unsigned long drawPrevTime = 0;
    volatile unsigned long drawTimeDx = 0;

    void (*drawCb) (void);

private:


// defaults
    void drawTextAlignRight(String text, int x, int y, int size, boolean showDec) {
        if (showDec) {
            if (text.length() == 1) {
                text = "0" + text;
            }
        }
        int len = text.length() + 1;
        int dx = this->FONT_SIZE * size;
        int pointPos = len - 3;
        char copy[len];
        text.toCharArray(copy, len);
        for (int i = len - 1; i >= 0; i--) {
            if (showDec && i == pointPos) {
                display.drawChar(x, y, '.', 1, 0, size);
                x = x - dx;
            }
            char ch = copy[i];
            display.drawChar(x, y, ch, 1, 0, size);
            x = x - dx;
        }
    }

    void drawTextAlignLeft(String text, int x, int y, int size) {
        display.setCursor(x, y);
        display.setTextSize(size);
        display.println(text);
//        int len = text.length() + 1;
//        int dx = this->FONT_SIZE * size;
//        char copy[len];
//        text.toCharArray(copy, len);
//        for (int i = 0; i < len; i++) {
//            char ch = copy[i];
//            display.drawChar(x, y, ch, 1, 0, size);
//            x = x + dx;
//        }
    }

public:
    volatile unsigned long drawLastTime = 0;
    volatile float fps = 0;

    void setupDisplay(String ver) {
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(5, 5);
        display.println(ver);
        display.display();
        delay(1000);
    }

    void onDraw( void(*func)(void)) {
        drawCb = func;
    }


    void drawScreen() {
        this->drawLastTime = millis();
        this->drawTimeDx = getTimeDiff(this->drawLastTime, this->drawPrevTime);

        if (this->drawTimeDx > this->drawEveryThMs) {
            display.clearDisplay();
            display.setTextColor(WHITE);

            this->drawCb();

            this->fps = 1000 / this->drawTimeDx;

            display.display();
            this->drawPrevTime = this->drawLastTime;
        }
    }

    // right places
    void drawIntMain(String val) {
        this->drawTextAlignRight(val, 120, 6, 2, false);
    }

    void drawIntSecond(String val) {
        this->drawTextAlignRight(val, 118, 24, 1, false);
    }

    void drawDistanceMain(String val) {
        this->drawTextAlignRight(val, 120, 6, 2, true);
    }

    void drawDistanceSecond(String val) {
        this->drawTextAlignRight(val, 118, 24, 1, true);
    }

// left places
    void drawLeftTopTitle(String val) {
        this->drawTextAlignLeft(val, 0, 6, 1);
    }

    void drawLeftCenterTitle(String val) {
        this->drawTextAlignLeft(val, 0, 14, 1);
    }

    void drawLeftBottomTitle(String val) {
        this->drawTextAlignLeft(val, 0, 22, 1);
    }

};