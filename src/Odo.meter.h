#include <Wire.h>

unsigned long MT_LAT = 10000; //10 ms

int aMaster = 0x50;
int aSlave = 0x57;

int addr = 0x100;
int solt = 12345;

struct Dist {
    volatile unsigned long dist[6] = {
            0, // linked with circle len, for calculate correct distance
            0, // current distance
            0, // local distance
            0, // item info (Engine)
            0, // item info (Oil)
            0  // item info (Gear)
    };
    volatile int circleLen = 0;
    int crc = 0;
};

class Odometer {
private:
    int sensors = 1;

    volatile float rpmTimeProcess = 0;

    volatile unsigned long timePrev = 0;
    volatile unsigned long timeNext = 0;
    volatile unsigned long timeDXTmp = 0;
    volatile unsigned long timeDXSpeed = 0;

    volatile Dist dat;
    unsigned long prevSaveTime = 0;
    unsigned long saveTime = 0;
    unsigned long checkLen = 0;
    void pack(int id, int addr, void *data, int len) {
        Wire.beginTransmission(id);
        Wire.write((byte * ) & addr, 2);
        Wire.write((byte *) data, len);
        Wire.endTransmission(true);
    }

    int unpack(int id, int addr, void *data, int len) {
        int rc;
        byte *p;
        Wire.beginTransmission(id);
        Wire.write((byte * ) & addr, 2);
        Wire.endTransmission(false);
        Wire.requestFrom(id, len);
        for (rc = 0, p = (byte *) data; Wire.available() && rc < len; rc++, p++) {
            *p = Wire.read();
        }
        return (rc);
    }


public:

    String version = "Odo.meter v 0.0.5";
    volatile boolean canSave = false;
    volatile boolean isCalc = true;
    volatile unsigned long timeDX = 0;
    volatile long rpmVal = 0;
    volatile long rpmMaxVal = 0;
    volatile int speedVal = 0;
    volatile long turnCount = 0;
    volatile long speedDx = 0;

    void readData() {
        this->unpack(aMaster, addr, (void *) &this->dat, sizeof(struct Dist));
//        Serial.println("Read from master; size =" + String(sizeof(this->dat)));
//        this->printDists();
        if (this->dat.crc == solt) {
            // all ok
        } else {
            // read from slave
//            Serial.println("Read from slave; size=" + String(sizeof(this->dat)));
            this->unpack(aSlave, addr, (void *) &this->dat, sizeof(struct Dist));
            if (this->dat.crc != solt) {
                this->dropData();
            }
//            this->printDists();
        }
    }

    void printDists() {
        Serial.println("------------");
        for (int i = 0; i < 6; i++) {
            Serial.println(String("pos = ") + String(i) + " val = " + String(this->dat.dist[i]));
        }
        Serial.println("circlelen = " + String(this->dat.circleLen));
        Serial.println("crc = " + String(this->dat.crc));
        Serial.println("------------");
    }

    void saveData() {
        this->saveTime = millis();
        // not faster than 2 sec!!! exclude force flag
        if (this->saveTime >= this->prevSaveTime + 2000) {
            this->saveImmediate();
        }
    }

    void saveImmediate() {
        this->dat.crc = solt;
//        this->printDists();
        this->pack(aMaster, addr, (void *) &this->dat, sizeof(struct Dist));
        delay(50);
        this->pack(aSlave, addr, (void *) &this->dat, sizeof(struct Dist));
        this->prevSaveTime = this->saveTime;
        this->canSave = false;
    }

    void dropData() {
        for (int i = 0; i < 6; i++) {
            this->dat.dist[i] = 0;
        }
        this->dat.circleLen = 2048;
    }

    void toSave() {
        this->canSave = true;
    }

    void init(int sens) {
        Wire.begin();
        this->sensors = sens;
        this->rpmTimeProcess = (60 / sens) * 1000000; // using micro seconds (rotations per minute)
    }

    void calcTimeDiff() {
        this->timeNext = micros();
        this->timeDXTmp = getTimeDiff(this->timeNext, this->timePrev);
        if (this->timeDXTmp < MT_LAT) {
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
        } else {
            this->speedVal = 0;
        }
    }

    void decSpeed() {
        this->timeDXSpeed = getTimeDiff(micros(), this->timePrev);
        if (this->timeDXSpeed > this->timeDX * 2) {
            (this->speedVal > 0) && this->speedVal--;
        }
    }

    void calcDist() {
        this->dat.dist[0] += this->dat.circleLen;
        if (this->dat.dist[0] > DIST_MAX_COUNT_LOCALE) {
            //increment counters and drop incrementer
            this->checkLen = this->dat.dist[1];
            for (int i = 1; i < 6; i++) {
                this->dat.dist[i]++;
            }

            if (this->checkLen > this->dat.dist[1]) {
//                unsigned long stopTime = millis();
//                Serial.println("stopTime = " + String(stopTime));
//                Serial.println("this->checkLen = " + String(this->checkLen));
//                Serial.println("this->dat.dist[1] = " + String(this->dat.dist[1]));
            }

            this->dat.dist[0] -= DIST_MAX_COUNT_LOCALE;
            this->turnDrop();
        }
    }

    void turnInc() {
        this->turnCount++;
    }

    void turnDec() {
        (this->turnCount > 0) && this->turnCount--;
    }

    void turnDrop() {
        this->turnCount = 0;
    }

    void enableCalc() {
        this->isCalc = true;
    }

    void disableCalc() {
        this->isCalc = false;
    }

    void setDiam(long val) {
        this->setCircle(round(PI * val));
    }

    void setCircleLen(long val) {
        this->dat.circleLen = val;
        this->setCircle(val);
    }

    unsigned long getCircleLen() {
        return this->dat.circleLen;
    }

    void setCircle(long val) {
        this->dat.circleLen = val;
        this->speedDx = val * KMH;
    }

    long getDist(int pos) {
        return this->dat.dist[pos];
    }

    void setDist(int pos, long val) {
        this->dat.dist[pos] = val;
    }

};
