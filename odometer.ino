#include "src/utils.h"
#include "src/Odo.meter.h"
#include "src/Menu.h"
#include "src/DisplayOled-64.h"

int PIN_BTN = A1;
int PIN_HALL_SENSOR = 2; // define the Hall magnetic sensor interface
int TOTAL_SENSORS = 1; // how much hall sensors we have //:todo add to menu define number of sensors
int INTERRUPT_NUM = 0;

DisplayOled displayOled;

Odometer odo;


MenuOdo menuOdo(PIN_BTN);

int cnt = 0;


void setup() {
    Serial.begin(9600);

    odo.init(TOTAL_SENSORS);

    pinMode(PIN_HALL_SENSOR, INPUT);  // define the Hall magnetic sensor line as input
    attachInterrupt(INTERRUPT_NUM, onHall, FALLING); // define interrupt for hall sensor

    menuOdo.initButton();

    odo.setDiam(640); // 64 cm as default


    //odo.dropData(); // drop all stored data

    delay(50);
    odo.readData();//init data from fram memory


    displayOled.setupDisplay(odo.version, 10);
    setupMenuItems();

    Serial.println("App started! ver = " + odo.version);
    delay(2000);
    displayOled.clearDisplay();
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
    cnt++;
    odo.calcTimeDiff();
    odo.turnInc();
    if (odo.isCalc) {
        odo.toSave();
        odo.calcDist();
        odo.calcRpm();
        odo.calcSpeed();
    }
    if (cnt == 100) {
        odo.printDists();
        cnt = 0;
    }
}

void drawMenuItem() {
    menuOdo.drawCurrentItem();
}

void onMenuItemChanged() {
    displayOled.clearDisplay();
}

