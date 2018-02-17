#include "ItemMenuBase.h"

int HOLD_COUNT_MAX = 10;

// axises
int range = 12;               // output range of X or Y movement
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value
int readAxis(int thisAxis) {
    // read the analog input:
    int reading = analogRead(thisAxis);

    // map the reading from the analog input range to the output range:
    reading = map(reading, 0, 1023, 0, range);

    // if the output reading is outside from the rest position threshold, use it:
    int distance = reading - center;

    if (abs(distance) < threshold) {
        distance = 0;
    }

    // return the distance for this axis:
    return distance;
}

class MenuOdo {
    int pinBtnCenter = 10;

    int lastBtn = ItemMenuBase::btnNone;

    unsigned long btnHoldTimeStart = 0;
    unsigned long btnHoldTimeDx = 0;
    boolean btnPressed = false;

    unsigned long holdTimeValue = 500; // in ms
    unsigned long lowTimeValue = 500; // in ms
    unsigned long now = 0;
    unsigned long lowSpeedTime = 0;
    float lowDx = 0;
    int holdCounter = 1;

    void (*naviCb) (void);

private:

    void processHoldClicks(int btn, ItemMenuBase *currItem) {
        if (this->holdCounter < HOLD_COUNT_MAX) {
            // low speed
            this->now = millis();

            this->lowDx = this->lowTimeValue * (this->holdCounter * ((HOLD_COUNT_MAX - this->holdCounter) * 10 / HOLD_COUNT_MAX)) / 10;
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
        int ret = ItemMenuBase::btnNone;

        (digitalRead(this->pinBtnCenter) != HIGH) && (ret = ItemMenuBase::btnCenter);

        int xReading = readAxis(A0);
        int yReading = readAxis(A1);

        (xReading > 0 || yReading > 0) && (ret = ItemMenuBase::btnUp);
        (xReading < 0 || yReading < 0) && (ret = ItemMenuBase::btnDown);

        return ret;
    }

public:
    MenuOdo(int pin);

    void onNavi( void(*func)(void)) {
        naviCb = func;
    }

    void onItemMenu(ItemMenuBase *currItem) {
        int btn = this->getBtn();

        if (btn != ItemMenuBase::btnNone) {
            if (this->btnHoldTimeStart == 0) {
                this->btnHoldTimeStart = millis();
                // button pressed
                currItem->onKeyDown(btn);
                this->lastBtn = btn;
                this->btnPressed = true;
            } else {
                this->btnHoldTimeDx = getTimeDiff(millis(), btnHoldTimeStart);
                if (this->btnPressed && (this->btnHoldTimeDx > this->holdTimeValue)) {
                    // hold press
                    if (btn == ItemMenuBase::btnCenter) {
                        this->naviCb();
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
                    currItem->onKeyUp(lastBtn);
                }
                this->holdCounter = 1;
                this->btnHoldTimeDx = 0;
                this->btnHoldTimeStart = 0;
            }
        }
    }

};

MenuOdo::MenuOdo(int pin) {
    pinBtnCenter = pin;
}
