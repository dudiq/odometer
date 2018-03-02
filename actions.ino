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
        displayOled.drawLeftBottomTitle(String(odo.speedVal));
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            // inc common dist
            odo.setDist(2, distInc(odo.getDist(2), this->dxDistance));
            odo.toSave();
        } else if (btn == ItemMenuBase::btnDown) {
            // decrement common dist
            odo.setDist(2, distDec(odo.getDist(2), this->dxDistance));
            odo.toSave();
        } else if (btn == ItemMenuBase::btnCenter) {
            // drop local distance
            odo.setDist(1, 0);
            odo.toSave();
        }
    }

    virtual void onKeyHold(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            odo.setDist(2, distInc(odo.getDist(2), this->hardDxDist));
        } else if (btn == ItemMenuBase::btnDown) {
            odo.setDist(2, distDec(odo.getDist(2), this->hardDxDist));
        }
    }
};

class ItemOneKilometer : public ItemMenuBase {
public:

    virtual void onInit() {
        odo.turnDrop();
        odo.disableCalc();
        this->dropSave();
    }

    virtual void onLeave() {
        odo.enableCalc();
        if (this->canSave) odo.toSave();
    }

    virtual void draw() {
        // left elements
        displayOled.drawLeftBottomTitle("One km");
        displayOled.drawIntMain(String(odo.turnCount));
    }
    
    void turnBy(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            odo.turnInc();
        } else if (btn == ItemMenuBase::btnDown) {
            odo.turnDec();
        }
    }

    virtual void onKeyUp(int &btn) {
        this->turnBy(btn);
        if (btn == ItemMenuBase::btnCenter) {
            long curr = (1000L * ONE_METER) / odo.turnCount;
            odo.setCircleLen(curr);
            this->setSave();
        }
    }

    virtual void onKeyHold(int &btn) {
        this->turnBy(btn);
    }

};

class ItemSetDiameter : public ItemMenuBase {
private:
    unsigned long curr = 0;
public:

    virtual void onInit() {
        this->curr = round(odo.getCircleLen() / PI);
        this->dropSave();
    }

    virtual void onLeave() {
        if (this->canSave) odo.toSave();
    }

    virtual void draw() {
        // left elements
        displayOled.drawLeftBottomTitle("Diam, mm");

        displayOled.drawIntMain(String(this->curr));
        displayOled.drawIntSecond(String(round(odo.getCircleLen() / PI)));
    }

    virtual void onBtns(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            this->curr++;
        } else if (btn == ItemMenuBase::btnDown) {
            (this->curr > 0) && (this->curr--);
        }
    }

    virtual void onKeyHold(int &btn) {
        this->onBtns(btn);
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnCenter) {
            odo.setDiam(this->curr);
            this->setSave();
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
        this->curr = odo.getCircleLen();
        this->dropSave();
    }

    virtual void onLeave() {
        if (this->canSave) odo.toSave();
    }

    virtual void draw() {
        // left elements
        displayOled.drawLeftBottomTitle("Len, mm");
        displayOled.drawIntMain(String(this->curr));
        displayOled.drawIntSecond(String(odo.getCircleLen()));
    }

    virtual void onBtns(int &btn) {
        if (btn == ItemMenuBase::btnUp) {
            this->curr++;
        } else if (btn == ItemMenuBase::btnDown) {
            (this->curr > 0) && this->curr--;
        }
    }

    virtual void onKeyHold(int &btn) {
        this->onBtns(btn);
    }

    virtual void onKeyUp(int &btn) {
        if (btn == ItemMenuBase::btnCenter) {
            odo.setCircleLen(this->curr);
            this->setSave();
        } else {
            this->onBtns(btn);
        }
    }

};

class ItemInfo : public ItemMenuBase {
private:
    int pos = 0;
    int dx = 3;
    String titles[3] = { // max 3!!!
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
            (this->pos <= 0) ? (this->pos = 2) : this->pos--;
        } else if (btn == ItemMenuBase::btnDown) {
            (this->pos >= 2) ? (this->pos = 0) : this->pos++;
        } else if (btn == ItemMenuBase::btnCenter) {
            odo.setDist(this->pos + this->dx, 0);
            odo.toSave();
        }
    }

};
