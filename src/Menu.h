#include "ItemMenuBase.h"

int HOLD_COUNT_MAX = 10;
int btnData;
int btnResult;
int menuItemPos;

class MenuOdo {
    int lastBtn = ItemMenuBase::btnNone;
    int pinRead;

    unsigned long btnHoldTimeStart = 0;
    unsigned long btnHoldTimeDx = 0;
    boolean btnPressed = false;

    unsigned long holdTimeValue = 500; // in ms
    unsigned long lowTimeValue = 500; // in ms
    unsigned long now = 0;
    unsigned long lowSpeedTime = 0;
    float lowDx = 0;
    int holdCounter = 1;

    int posItem = 0;
    int totalItems = 10;
    ItemMenuBase *menu[10] = {};

    void (*onChangedCb)(void);

private:

    void processHoldClicks(int btn, ItemMenuBase *currItem) {
        if (this->holdCounter < HOLD_COUNT_MAX) {
            // low speed
            this->now = millis();

            this->lowDx = this->lowTimeValue *
                          (this->holdCounter * ((HOLD_COUNT_MAX - this->holdCounter) * 10 / HOLD_COUNT_MAX)) / 10;
            this->lowSpeedTime = this->btnHoldTimeStart + this->holdTimeValue;
            this->lowSpeedTime += this->lowDx;

            if (this->lowSpeedTime < this->now) {
                currItem->onKeyHold(btn);
                this->holdCounter++;
            }
        } else {
            // high speed
            currItem->onKeyHold(btn);
        }
    }

    int getBtn() {
        btnResult = ItemMenuBase::btnNone;
        btnData = analogRead(pinRead);
        //Serial.println("--| " + String(btnData));

        // btn results from analog read
        // 72 - up
        // 123 - center
        // 169 - down
        // 210 - menu

        if (btnData > 50 && btnData < 100) {
            btnResult = ItemMenuBase::btnUp;
        } else if (btnData > 100 && btnData < 140) {
            btnResult = ItemMenuBase::btnCenter;
        } else if (btnData > 140 && btnData < 185) {
            btnResult = ItemMenuBase::btnDown;
        } else if (btnData > 185 && btnData < 230) {
            btnResult = ItemMenuBase::btnMenu;
        }

        return btnResult;
    }

    void processNavi() {
        menuItemPos = this->posItem;
        this->menu[menuItemPos]->onLeave();
        menuItemPos++;
        if (menuItemPos >= this->totalItems) {
            menuItemPos = 0;
        }
        this->posItem = menuItemPos;
        this->onChangedCb();
        menu[menuItemPos]->onInit();
    }

public:
    MenuOdo(int pin);

    void initButton() {
        pinMode(this->pinRead, INPUT_PULLUP);
    }

    void onChanged(void(*func)(void)) {
        onChangedCb = func;
    }

    void initMenuItems(ItemMenuBase *menu[], int total) {
        for (int i = 0; i < total; i++) {
            this->menu[i] = menu[i];
        }
        this->totalItems = total;
    }

    void onItemMenu() {
        ItemMenuBase *currItem = this->menu[this->posItem];
        int btn = this->getBtn();

        if (btn != ItemMenuBase::btnNone) {
            if (this->btnHoldTimeStart == 0) {
                this->btnHoldTimeStart = millis();
                // button pressed
                this->lastBtn = btn;
                this->btnPressed = true;
                currItem->onKeyDown(btn);
            } else {
                this->btnHoldTimeDx = getTimeDiff(millis(), btnHoldTimeStart);
                if (this->btnPressed && (this->btnHoldTimeDx > this->holdTimeValue)) {
                    // hold press
                    if (btn == ItemMenuBase::btnCenter) {
                        this->processNavi();
                        this->btnPressed = false;
                    } else {
                        this->processHoldClicks(btn, currItem);
                    }
                }
            }
        } else {
            if (this->btnHoldTimeStart != 0) {
                if (this->btnHoldTimeDx <= this->holdTimeValue) {
                    // release
                    // if hold processed, onUp NOT call
                    if (lastBtn == ItemMenuBase::btnMenu) {
                        // use navi
                        this->processNavi();
                    } else {
                        currItem->onKeyUp(lastBtn);
                    }
                }
                this->holdCounter = 1;
                this->btnHoldTimeDx = 0;
                this->btnHoldTimeStart = 0;
            }
        }
    }

    void drawCurrentItem() {
        this->menu[this->posItem]->draw();
    }

};

MenuOdo::MenuOdo(int pin) {
    this->pinRead = pin;
}
