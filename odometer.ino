#include "src/utils.h"
#include "src/Odo.meter.h"
#include "src/Menu.h"
#include "src/DisplayOled.h"

int PIN_BTN_CENTER = 10; // button
int PIN_HALL_SENSOR = 2; // define the Hall magnetic sensor interface
int TOTAL_SENSORS = 1; // how much hall sensors we have //:todo add to menu define number of sensors

DisplayOled displayOled;

Odometer odo;

//Storage store;

MenuOdo menuOdo(PIN_BTN_CENTER);


void setup() {
    odo.init(TOTAL_SENSORS);
//    store.init();
    
    pinMode(PIN_BTN_CENTER, INPUT_PULLUP); // button
    pinMode(PIN_HALL_SENSOR, INPUT);  // define the Hall magnetic sensor line as input

    attachInterrupt(0, onHall, FALLING);
    odo.setDiam(640); // 64 cm as default

    Serial.begin(9600);

    //odo.dropData();
    delay(50);
    odo.readData();//init data from fram memory
    
    Serial.println("App started! ver = " + odo.version);

    displayOled.setupDisplay(odo.version, 10);
    initMenu();
}

void loop() {
    menuOdo.onItemMenu();
    displayOled.drawScreen();
    delay(50);
    odo.decSpeed();
    if (odo.canSave) {
        odo.saveData();
    };
}


void onHall() {
    odo.calcTimeDiff();
    odo.turnInc();
    if (odo.isCalc) {
        odo.toSave();
        odo.calcDist();
        odo.calcRpm();
        odo.calcSpeed();
    }
}
  

