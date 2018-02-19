
MenuOdo menuOdo(PIN_BTN_CENTER);

ItemMenuBase *menu[] = {
        new ItemApp(),
        new ItemOneKilometer(),
        new ItemSetDiameter(),
        new ItemSetLength(),
        new ItemInfo()
};

int menuItemPos = 0;

void onItemMenu() {
    menuOdo.onItemMenu(menu[menuItemPos]);
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
