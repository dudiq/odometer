#include "src/utils.h"
#include "src/Odo.meter.h"
#include "src/Menu.h"
#include "src/DisplayOled.h"
#include "src/StoreFram.h"

int PIN_BTN_CENTER = 10; // button
int PIN_HALL_SENSOR = 2; // define the Hall magnetic sensor interface
int TOTAL_SENSORS = 1; // how much hall sensors we have //:todo add to menu define number of sensors

DisplayOled displayOled;

Odometer odo(TOTAL_SENSORS);

Storage store;

void setup() {

    pinMode(PIN_BTN_CENTER, INPUT_PULLUP); // button
    pinMode(PIN_HALL_SENSOR, INPUT);  // define the Hall magnetic sensor line as input

    attachInterrupt(0, onHall, FALLING);
    odo.setDiam(640); // 64 cm as default

    Serial.begin(9600);


    Serial.println("App started! ver = " + odo.ver());
    store.init();
    displayOled.setupDisplay(odo.ver());
    initMenu();
}

void loop() {
    onItemMenu();
    displayOled.drawScreen();
    delay(50);
}

void onHall() {
    odo.calcTimeDiff();
    odo.turnInc();
    if (odo.isCalc) {
        odo.calcDist();
        odo.calcRpm();
        odo.calcSpeed();
    }
}
