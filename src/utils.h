unsigned long MAX_UNSIGNED = 4294967295;
unsigned long HALF_UNSIGNED = MAX_UNSIGNED / 2;

int ONE_CENTIMETER = 10; // used in mm as start point, stored data in mm
int ONE_METER = ONE_CENTIMETER * 10; // 1m

unsigned long DIST_MAX_COUNT_LOCALE = 1000L * ONE_METER; // 1000 meters
int DISTANCE_COUNTER_INCREMENT = 1; // as single point

int KMH = 3600;

unsigned long distInc(unsigned long dist, int dx) {
    if ((dist + dx) > dist) {
        dist = dist + dx;
    }
    return dist;
}

unsigned long distDec(unsigned long dist, int dx) {
    if ((dist - dx) < dist) {
        dist = dist - dx;
    } else {
        dist = 0;
    }
    return dist;
}

unsigned long getTimeDiff(unsigned long startTime, unsigned long endTime) {
    unsigned long dx = 0;
    if (endTime > startTime) {
        if (endTime > HALF_UNSIGNED) {
            dx = (MAX_UNSIGNED - endTime) + startTime;
        } else {
            dx = endTime - startTime;
        }
    } else {
        dx = startTime - endTime;
    }
    return dx;
}
