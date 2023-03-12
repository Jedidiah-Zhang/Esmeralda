#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("://resources/images/blocks_icon.png"));

    /*==========Scenes==========*/
    // set up all the scenes, and determine which scene is shown.
    // show only one scene at a time.
    this->menuList = new QStackedWidget(this);

    //note that the order of scenes added to menuList is the same as the enum SCENES
    this->levelSelectScene = new levelSelect(this);
    this->minigameScene = new Games(this);
    this->designScene = new Design(this);
    this->progressScene = new Progress(this);
    this->menuScene = new Menu(this);
    this->buildScene = new Build(this);

    this->menuList->addWidget(this->levelSelectScene);
    this->menuList->addWidget(this->minigameScene);
    this->menuList->addWidget(this->designScene);
    this->menuList->addWidget(this->progressScene);
    this->menuList->addWidget(this->menuScene);
    this->menuList->addWidget(this->buildScene);

    this->menuList->setCurrentIndex(MENU);
    setCentralWidget(this->menuList);

    /*==========Connections==========*/
    // menu scene
    connect(this->menuScene, SIGNAL(leftBtnClicked(int)), this, SLOT(changeScene(int)));
    connect(this->menuScene, SIGNAL(rightBtnClicked(int)), this, SLOT(changeScene(int)));
    connect(this->menuScene, &Menu::centralBtnClicked, this, [=](){
        this->menuList->setCurrentIndex(LEVELSELECT);
    });

    // level select scene
    connect(this->levelSelectScene, &levelSelect::backButtonClicked, this, [=](){
        this->menuList->setCurrentIndex(MENU);
    });
    connect(this->levelSelectScene, SIGNAL(levelSelected(int)), this, SLOT(toBuildScene(int)));

    // build scene
    connect(this->buildScene, &Build::backButtonClicked, this, [=](){
        this->menuList->setCurrentIndex(LEVELSELECT);
    });
    connect(this->buildScene, &Build::updateRecordFilesReady, this, &MainWindow::updateRecordFiles);

    // design scene
    connect(this->designScene, &Design::backButtonClicked, this, [=](){
        this->menuList->setCurrentIndex(MENU);
    });

    // mini game scene
    connect(this->minigameScene, &Games::backButtonClicked, this, [=](){
        this->menuList->setCurrentIndex(MENU);
    });

    // progress scene
    connect(this->progressScene, &Progress::backButtonClicked, this, [=](){
        this->menuList->setCurrentIndex(MENU);
    });

    /*==========FileReading==========*/
    // load levels
    this->levels = new QMap<int, QVector<Block> *>;
    QDir dirLevel("../BlockCraft/levels/");
    dirLevel.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList lvlList = dirLevel.entryInfoList();
    for (int i = 0; i < lvlList.count(); i++) {
        QFileInfo info = lvlList.at(i);
        QFile file(info.absoluteFilePath());
        readLevelFile(info.absoluteFilePath(), info.baseName());
    }
    this->levelSelectScene->setLevels(this->levels->count());

    // load records
    this->records = new QMap<int, QVector<Record> *>;
    QDir dirRecord("../BlockCraft/records/");
    dirRecord.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList rcdList = dirRecord.entryInfoList();
    // go through every file
    for(int i = 0; i < rcdList.count(); i++) {
        QFileInfo info = rcdList.at(i);
        QFile file(info.absoluteFilePath());
        readRecordFile(info.absoluteFilePath(), info.baseName());
    }

    /*==========BlueTooth==========*/
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    discoveryAgent->setLowEnergyDiscoveryTimeout(15000);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(discoverBlueTooth(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));
    discoveryAgent->start();

}

// switch the function of the central button
void MainWindow::changeScene(int idx)
{
//    qDebug() << idx;
    this->menuScene->disconnect(SIGNAL(centralBtnClicked()));
    connect(this->menuScene, &Menu::centralBtnClicked, this, [=](){
        this->menuList->setCurrentIndex(SCENES(idx));
    });
}

// switch to build scene, with specified level
void MainWindow::toBuildScene(int lvl)
{
    this->menuList->setCurrentIndex(BUILD);
    this->buildScene->setLevel(lvl);
}

// tell every scene that the window size is changed
// hence to rearrange widgets on them
void MainWindow::resizeEvent(QResizeEvent *)
{
    this->menuScene->determineGeometry();
    this->levelSelectScene->determineGeometry();
}

void MainWindow::readRecordFile(QString fileDir, QString level)
{
    int minTime = INT_MAX;

    QFile file(fileDir);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        qDebug() << "error: " << parseError.error;
    QJsonObject obj = doc.object();
    if (obj.contains(level)) {
        QJsonArray array = obj.value(level).toArray();
        QVector<Record> * singleLevelRecord = new QVector<Record>;
        for (int j = 0; j < array.count(); j++) {
            QJsonObject subObj = array.at(j).toObject();
            Record record;
            record.startTime = subObj.value("startTime").toInt();
            record.timeUsed = subObj.value("timeUsed").toInt();
            record.attempts = subObj.value("attempts").toInt();
            singleLevelRecord->push_back(record);

            minTime = minTime < record.timeUsed ? minTime : record.timeUsed;
        }
        this->records->insert(level.toInt(), singleLevelRecord);

        this->levelSelectScene->setStarRecords(level.toInt(), minTime);
    }
}

void MainWindow::readLevelFile(QString fileDir, QString level)
{
    QFile file(fileDir);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        qDebug() << "error: " << parseError.error;
    QJsonObject obj = doc.object();
    if (obj.contains(level)) {
        QJsonArray array = obj.value(level).toArray();
        QVector<Block> * blocks = new QVector<Block>;
        for (int j = 0; j < array.count(); j++) {
            QJsonObject blockObj = array.at(j).toObject();
            Block block;
            block.ID = blockObj.value("ID").toInt();
            block.x = blockObj.value("x").toInt();
            block.z = blockObj.value("z").toInt();
            block.y = blockObj.value("y").toInt();
            block.rot = blockObj.value("rot").toInt();
            blocks->push_back(block);
        }
        this->levels->insert(level.toInt(), blocks);
    }
}

void MainWindow::updateRecordFiles(int idx, int curT, int useT, int Att)
{
    QVector<Record> *singleLevelRecord;
    Record record;
    record.timeUsed = useT;
    record.startTime = curT;
    record.attempts = Att;
    if (this->records->value(idx) == nullptr)
        singleLevelRecord = new QVector<Record>;
    else singleLevelRecord = this->records->value(idx);
    singleLevelRecord->push_back(record);

    QJsonObject obj;
    QJsonArray array;
    for (int i = 0; i < singleLevelRecord->size(); i++) {
        QJsonObject sub;
        sub.insert("startTime", singleLevelRecord->at(i).startTime);
        sub.insert("timeUsed", singleLevelRecord->at(i).timeUsed);
        sub.insert("attempts", singleLevelRecord->at(i).attempts);
        array.append(QJsonValue(sub));
    }
    obj.insert(QString("%1").arg(idx), QJsonValue(array));

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    QDir dir = QDir("../BlockCraft/records/");
    if (!dir.exists()) dir.mkpath(".");
    QFile file(QString("%1/%2.json").arg(dir.absolutePath()).arg(idx));
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(data);
        file.close();
    } else qDebug() << "Write error" << file.errorString();

    int minTime = INT_MAX;
    for (int i = 0; i < this->records->value(idx)->length(); i++) {
        int time = this->records->value(idx)->at(i).timeUsed;
        minTime = minTime < time ? minTime : time;
    }
    this->levelSelectScene->setStarRecords(idx, minTime);
}

void MainWindow::discoverBlueTooth(QBluetoothDeviceInfo info)
{
    QString label = QString("%1 %2").arg(info.address().toString(), info.name());
    if (info.name() == "HC-05") this->BTaddress = info.address().toString();
    qDebug() << label;
}

void MainWindow::scanFinished()
{
    qDebug() << "Scan Finished";
    static QString serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    this->socket->connectToService(QBluetoothAddress(this->BTaddress), QBluetoothUuid(serviceUuid),QIODevice::ReadWrite);
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readBluetoothData()));
    connect(this->socket, SIGNAL(connected()), this, SLOT(bluetoothConnected()));
}

void MainWindow::readBluetoothData()
{
    char data[100];
    qint64 len = socket->read((char *)data, 100);

    QByteArray qa2((char*)data ,len);
    QString qstr(qa2.toHex());
    qDebug()<<"----" << qstr;
}

void MainWindow::sendBluetoothData(Package package)
{
    switch (package.cmd) {
    case AUDIO:
        break;
    case LED:
        break;
    default:
        break;
    }

    qDebug() << "Send Data";
    // socket->write(data);
}

void MainWindow::bluetoothConnected()
{
    qDebug() << "Bluetooth Connected.";
}

MainWindow::~MainWindow()
{
    delete ui;
}

