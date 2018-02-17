class ItemMenuBase {
private:
    int naviState = 0;
public:

    static const int naviNone = 0;
    static const int naviUp = 1;
    static const int naviDown = 2;
    static const int naviExit = 3;
    static const int naviEnter = 4;

    static const int btnNone = 0;
    static const int btnUp = 1;
    static const int btnDown = 2;
    static const int btnCenter = 3;

    inline virtual void draw();

    inline virtual void onKeyDown(int &);

    inline virtual void onKeyHold(int &);

    inline virtual void onKeyUp(int &);

    inline virtual void onInit();

    inline virtual void onLeave();

};

void ItemMenuBase::draw() {
    Serial.println("empty item");
}


void ItemMenuBase::onKeyDown(int &) {

}

void ItemMenuBase::onKeyHold(int &) {

}

void ItemMenuBase::onKeyUp(int &) {

}

void ItemMenuBase::onInit() {

}

void ItemMenuBase::onLeave() {

}
