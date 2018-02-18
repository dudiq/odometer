
// items

class ItemApp : public ItemMenuBase {
private:
    int dxDistance = 1;
    int hardDxDist = 10;
public:
    virtual void draw() {
        // draw default state
        displayOled.drawDistanceMain(String(odo.getDist(1)));
        displayOled.drawDistanceSecond(String(odo.getDist(2)));

        // left elements

        int seconds = round(displayOled.drawLastTime / 1000);
        displayOled.drawLeftTopTitle("t=" + String(seconds));
        displayOled.drawLeftCenterTitle("fps=" + String(round(displayOled.fps)));
        displayOled.drawLeftBottomTitle("spd=" + String(odo.speedVal));
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            // inc common dist
            odo.setDist(distInc(odo.getDist(2), this->dxDistance), 2);
        } else if (btn == ItemMenuBase::btnDown) {
            // decrement common dist
            odo.setDist(distDec(odo.getDist(2), this->dxDistance), 2);
        } else if (btn == ItemMenuBase::btnCenter) {
            // drop local distance
            odo.setDist(1, 0);
        }
    }

    virtual void onKeyHold(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            odo.setDist(distInc(odo.getDist(2), this->hardDxDist), 2);
        } else if (btn == ItemMenuBase::btnDown) {
            odo.setDist(distDec(odo.getDist(2), this->hardDxDist), 2);
        }
    }

};

class ItemOneKilometer : public ItemMenuBase {
public:

    virtual void onInit() {
        odo.turnDrop();
        odo.disableCalc();
    }

    virtual void onLeave() {
        odo.enableCalc();
    }

    virtual void draw() {
        // left elements
        displayOled.drawLeftBottomTitle("One km");
        displayOled.drawIntMain(String(odo.turnCount));
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            odo.turnInc();
        } else if (btn == ItemMenuBase::btnDown) {
            odo.turnDec();
        } else if (btn == ItemMenuBase::btnCenter) {
            // :todo set value to
            long curr = (1000L * ONE_METER) / odo.turnCount;
            odo.setCircleLen(curr);
        }
    }

};

class ItemSetDiameter : public ItemMenuBase {
private:
    unsigned long curr = 0;
public:

    virtual void onInit() {
        this->curr = odo.circleDiameter;
    }

    virtual void draw() {
        // left elements
        displayOled.drawLeftBottomTitle("Diam, mm");

        displayOled.drawIntMain(String(this->curr));
        displayOled.drawIntSecond(String(odo.circleDiameter));
    }

    virtual void onBtns(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            this->curr++;
        } else if (btn == ItemMenuBase::btnDown) {
            this->curr--;
            (this->curr < 1) && (this->curr = 1);
        }
    }

    virtual void onKeyHold(int &btn) {
        this->onBtns(btn);
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnCenter) {
            odo.setDiam(this->curr);
        } else {
            this->onBtns(btn);
        }
    }

};

class ItemSetLength : public ItemMenuBase {
private:
    unsigned long curr = 0;
public:

    virtual void onInit() {
        this->curr = odo.circleLen;
    }

    virtual void draw() {
        // left elements
        displayOled.drawLeftBottomTitle("Len, mm");

        displayOled.drawIntMain(String(this->curr));
        displayOled.drawIntSecond(String(odo.circleLen));
    }

    virtual void onBtns(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            this->curr++;
        } else if (btn == ItemMenuBase::btnDown) {
            this->curr--;
            (this->curr < 1) && (this->curr = 0);
        }
    }

    virtual void onKeyHold(int &btn) {
        this->onBtns(btn);
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnCenter) {
            odo.setCircleLen(this->curr);
        } else {
            this->onBtns(btn);
        }
    }

};

class ItemInfo : public ItemMenuBase {
private:
    int pos = 0;
    int dx = 3;
    String titles[3] = { // max 6
            "Engine",
            "Oil",
            "Gear"
    };
public:

    virtual void onInit() {
        this->pos = 0;
    }

    virtual void draw() {
        // left elements
        String title = this->titles[this->pos];

        displayOled.drawLeftBottomTitle(title);

        displayOled.drawDistanceMain(String(odo.getDist(this->pos + this->dx)));
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            this->pos--;
            (this->pos < 0) && (this->pos = 2);
        } else if (btn == ItemMenuBase::btnDown) {
            this->pos++;
            (this->pos > 2) && (this->pos = 0);
        } else if (btn == ItemMenuBase::btnCenter) {
            odo.setDist(this->pos + this->dx, 0);
        }
    }

};
