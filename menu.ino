ItemMenuBase *menu[] = {
        new ItemApp(),
        new ItemOneKilometer(),
        new ItemSetDiameter(),
        new ItemSetLength(),
        new ItemInfo()
};

void drawMenuItem() {
    menuOdo.drawCurrentItem();
}

void initMenu() {
    menuOdo.initMenuItems(menu, 5);
    displayOled.onDraw(drawMenuItem);
}

