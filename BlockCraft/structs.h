#ifndef STRUCTS_H
#define STRUCTS_H

#include <QByteArray>

#define BTNAME "HC-05"

enum COMMANDS {
    STRUCT = 1,
    LED = 2,
    AUDIO = 3,
    COMPLET = 4,
    COMFIRMT = 0b01010001,
    COMFIRMF = 0b01010000
};

typedef struct Package {
    int cmd;
    QByteArray data;
} Package;

enum SCENES{
    LEVELSELECT = 0,
    GAME,
    DESIGN,
    PROGRESS,
    MENU,
    BUILD
};

typedef struct Block {
    int tile;
    int height;
    int ID;
    int rot;
} Block;

typedef struct Record {
    int startTime;
    int timeUsed;
    int attempts;
} Record;

#endif // STRUCTS_H
