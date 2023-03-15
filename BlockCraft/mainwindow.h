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

typedef struct Block {
    int ID;
    int x;
    int y;
    int z;
    int rot;
} Block;

typedef struct Record {
    int startTime;
    int timeUsed;
    int attempts;
} Record;

enum COMMANDS {
    BLOCKS,
    BUTTON,
    AUDIO,
    LED
};

typedef struct Package {
    int cmd;
    QByteArray data;
} Package;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void readRecordFile(QString fileDir, QString level);
    void readLevelFile(QString fileDir, QString level);

    ~MainWindow();

public slots:
    void changeScene(int idx);
    void toBuildScene(int lvl);
//    void passLevelData(int lvl);
    void updateRecordFiles(int idx, int curT, int useT, int Att);

private slots:
    void BTDiscover(QBluetoothDeviceInfo info);
    void BTConnect();
    void BTReSearching();
    void BTReadData();
    void BTSendData(Package package);
    void BTConnected();
    void BTUnableConnect(QBluetoothSocket::SocketError err);
    void sendAudioSucess();

signals:
    void deviceFound();

private:
//    void setAverageChart();

    virtual void resizeEvent(QResizeEvent *) override;

    Ui::MainWindow *ui;

    QStackedWidget *menuList = nullptr;
    Menu *menuScene = nullptr;
    levelSelect *levelSelectScene = nullptr;
    Games *minigameScene = nullptr;
    Design *designScene = nullptr;
    Progress *progressScene = nullptr;
    Build *buildScene = nullptr;

    QMap<int, QVector<Record> *> *records = nullptr;
    QMap<int, QVector<Block> *> *levels = nullptr;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QBluetoothLocalDevice *localDevice = nullptr;
    QString BTaddress;
    QBluetoothSocket *socket = nullptr;
};
#endif // MAINWINDOW_H
