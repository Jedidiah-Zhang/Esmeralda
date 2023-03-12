#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menu.h"
#include "levelselect.h"
#include "build.h"
#include "design.h"
#include "games.h"
#include "progress.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QVector>

#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <qbluetoothdevicediscoveryagent.h>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QBluetoothAddress>

#include <bitset>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum SCENES{
    LEVELSELECT = 0,
    GAME,
    DESIGN,
    PROGRESS,
    MENU,
    BUILD
};

typedef struct Record {
    int startTime;
    int timeUsed;
    int attempts;
} Record;

enum COMMANDS {

};

typedef struct Package {
    std::bitset<4> cmd;
    int lengh;

} Package;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void readRecordFile(QString fileDir, QString level);

    ~MainWindow();

public slots:
    void changeScene(int idx);
    void toBuildScene(int lvl);
    void updateRecordFiles(int idx, int curT, int useT, int Att);

private slots:
    void discoverBlueTooth(QBluetoothDeviceInfo info);
    void scanFinished();
    void readBluetoothData();
    void sendBluetoothData();
    void bluetoothConnected();

signals:

private:
    virtual void resizeEvent(QResizeEvent *) override;

    Ui::MainWindow *ui;

    QStackedWidget *menuList = nullptr;
    Menu *menuScene = nullptr;
    levelSelect *levelSelectScene = nullptr;
    Games *minigameScene = nullptr;
    Design *designScene = nullptr;
    Progress *progressScene = nullptr;
    Build *buildScene = nullptr;

    QMap<int, QVector<Record> *> *records;

    QBluetoothLocalDevice *localDevice;
    QString BTaddress;
    QBluetoothSocket *socket;
};
#endif // MAINWINDOW_H