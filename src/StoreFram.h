#include "FM24I2C.h"

FM24I2C fm(0x57);

class Storage {
public:

    void init(){
        Wire.begin();
    }

    void save() {
        char str1[] = "12345678901234567890";
        char str2[] = "qwertyuiopasdfghjklzxcvbnm";
        int a1 = 0x00;
        int a2 = 0x40;
        fm.pack(a1, str1, strlen(str1) + 1);
        delay(5);
        fm.pack(a2, str2, strlen(str2) + 1);
        delay(5);

    }

    void read() {
        char buf[80];
        char str1[] = "";
        char str2[] = "";
        int a1 = 0x00;
        int a2 = 0x40;
        fm.unpack(a2, buf, strlen(str2) + 1);
        Serial.println(str2);
        fm.unpack(a1, buf, strlen(str1) + 1);
        Serial.println(str1);
    }

};
