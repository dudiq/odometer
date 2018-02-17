unsigned long MIN_TIME_LATENCY = 10000; //10 ms

class Odometer {
private:
    String version = "Odo.meter v 0.0.3";
    int sensors = 1;

    float rpmTimeProcess = 0;

    volatile unsigned long timePrev = 0;
    volatile unsigned long timeNext = 0;
    volatile unsigned long timeDXTmp = 0;

    // distance counters, need to save this values
    long distances[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

public:
    Odometer(int sens, int dim);

    volatile boolean isCalc = true;
    volatile unsigned long timeDX = 0;
    volatile long rpmVal = 0;
    volatile long rpmMaxVal = 0;
    volatile int speedVal = 0;
    volatile long turnCount = 0;
    volatile long circleDiameter = 0;
    volatile long circleLen = 0;
    volatile long speedDx = 0;

    void calcTimeDiff() {
        this->timeNext = micros();
        this->timeDXTmp = getTimeDiff(this->timeNext, this->timePrev);
        if (this->timeDXTmp < MIN_TIME_LATENCY) {
            // drizzle of contacts or interrupts, noo need processing?
            this->timeDX = 0;
        }
        this->timeDX = this->timeDXTmp;
        this->timePrev = this->timeNext;
    }

    void calcRpm() {
        this->rpmVal = (this->timeDX > 0) ?
                       rpmTimeProcess / this->timeDX :
                       0;
        if (this->rpmMaxVal < this->rpmVal) {
            this->rpmMaxVal = this->rpmVal;
        }
    }

    void calcSpeed() {
        // timeDiff - micro seconds; 1 000 000 mcs = 1sec
        if (this->timeDX > 0) {
            this->speedVal = round(this->speedDx / this->timeDX); // km / h
        }
    }

    void calcDist() {
        this->distances[0] += this->circleLen;
        if (this->distances[0] > DIST_MAX_COUNT_LOCALE) {
            //increment counters and drop incrementer
            for (int i = 1; i < 10; i++) {
                this->distances[i]++;
            }

            this->distances[0] -= DIST_MAX_COUNT_LOCALE;
            this->turnDrop();
        }
    }

    void turnInc() {
        this->turnCount++;
    }

    void turnDec() {
        (this->turnCount < 1) ?
        (this->turnCount = 0) :
        this->turnCount--;
    }

    void turnDrop() {
        this->turnCount = 0;
    }

    String ver() {
        return this->version;
    }

    void enableCalc() {
        this->isCalc = true;
    }

    void disableCalc() {
        this->isCalc = false;
    }

    void setDiam(long val) {
        this->circleDiameter = val;
        this->setCircle(round(PI * val));
    }

    void setCircleLen(long val) {
        this->circleLen = val;
        this->circleDiameter = val / PI;
        this->setCircle(val);
    }

    void setCircle(long val){
        this->circleLen = val;
        this->speedDx = val * KMH;
    }

    long getDist(int pos) {
        return this->distances[pos];
    }

    void setDist(int pos, long val) {
        this->distances[pos] = val;
    }

};

Odometer::Odometer(int val, int diam) {
    sensors = val;
    rpmTimeProcess = (60 / val) * 1000000; // using micro seconds (rotations per minute)
    circleDiameter = ONE_CENTIMETER * diam;
    circleLen = round(PI * circleDiameter);
}