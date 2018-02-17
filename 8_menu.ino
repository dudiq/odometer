
MenuOdo menuOdo(PIN_BTN_CENTER);

ItemMenuBase *menu[] = {
        new ItemApp(),
        new ItemOneKilometer(),
        new ItemSetDiameter(),
        new ItemSetLength(),
        new ItemInfo()
};

int menuItemPos = 0;

void processBtnClick() {
    ItemMenuBase *currItem = menu[menuItemPos];
    // todo add timer for return to zero menu item after 20 secs if it not in oneKilometer
    menuOdo.onItemMenu(currItem);
}

void processNavi() {
    menu[menuItemPos]->onLeave();
    menuItemPos++;
    if (menuItemPos > 4) {
        menuItemPos = 0;
    }
    menu[menuItemPos]->onInit();
}

void drawMenu(){
    menu[menuItemPos]->draw();
}

void initMenu() {
    displayOled.onDraw(drawMenu);
    menuOdo.onNavi(processNavi);
}
