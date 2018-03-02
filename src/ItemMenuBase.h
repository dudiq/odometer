class ItemMenuBase {
public:
    boolean canSave = false;
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
    inline virtual void setSave() {
        this->canSave = true;
    }
    inline virtual void dropSave() {
        this->canSave = false;
    }
};

void ItemMenuBase::draw() {
    Serial.println("--");
}

void ItemMenuBase::onKeyDown(int &) {}

void ItemMenuBase::onKeyHold(int &) {}

void ItemMenuBase::onKeyUp(int &) {}

void ItemMenuBase::onInit() {}

void ItemMenuBase::onLeave() {}
