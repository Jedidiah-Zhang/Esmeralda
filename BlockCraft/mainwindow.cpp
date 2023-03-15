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
    connect(this->buildScene, &Build::updateRecordFilesReady, this, &MainWindow::sendAudioSucess);

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
//    connect(this->progressScene, SIGNAL(requestLevelData(int)), this, SLOT(passLevelData(int)));

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
//    QVector<int> *idxs = new QVector<int>;
    QDir dirRecord("../BlockCraft/records/");
    dirRecord.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList rcdList = dirRecord.entryInfoList();
    // go through every file
    for(int i = 0; i < rcdList.count(); i++) {
        QFileInfo info = rcdList.at(i);
        QFile file(info.absoluteFilePath());
        readRecordFile(info.absoluteFilePath(), info.baseName());
//        idxs->push_back(info.baseName().toInt());
    }
//    this->progressScene->setUpIndexes(idxs);
//    this->setAverageChart();

    /*==========BlueTooth==========*/
    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    this->discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(BTDiscover(QBluetoothDeviceInfo)));
    connect(this, &MainWindow::deviceFound, this, &MainWindow::BTConnect);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, menuScene, &Menu::BTSearchFailed);
    this->discoveryAgent->start();
    this->menuScene->BTSearching();

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
    QVector<int> pts;

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
            pts.push_back(record.timeUsed);
        }
        this->records->insert(level.toInt(), singleLevelRecord);

        this->levelSelectScene->setStarRecords(level.toInt(), minTime);
        this->progressScene->addChart(level.toInt(), pts);
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
    this->progressScene->addPoint(idx, useT);
}

void MainWindow::BTDiscover(QBluetoothDeviceInfo info)
{
    QString label = QString("%1 %2").arg(info.address().toString(), info.name());
    if (info.name() == "HC-05") {
        this->BTaddress = info.address().toString();
        emit deviceFound();
        this->discoveryAgent->stop();
    }
    qDebug() << label;
}

void MainWindow::BTConnect()
{
//    qDebug() << "Scan Finished";
    this->menuScene->BTConnecting();
    static QString serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    this->socket->connectToService(QBluetoothAddress(this->BTaddress), QBluetoothUuid(serviceUuid),QIODevice::ReadWrite);
    connect(this->socket, &QBluetoothSocket::readyRead, this, &MainWindow::BTReadData);
    connect(this->socket, &QBluetoothSocket::connected, this, &MainWindow::BTConnected);
    connect(this->socket, &QBluetoothSocket::disconnected, this, &MainWindow::BTReSearching);
    connect(this->socket, SIGNAL(errorOccurred(QBluetoothSocket::SocketError)),
            this, SLOT(BTUnableConnect(QBluetoothSocket::SocketError)));
}

void MainWindow::BTReSearching()
{
    this->menuScene->BTConnectionLost();

    QTimer::singleShot(3000, this, [=](){
        this->menuScene->BTSearching();
        this->discoveryAgent->start();
    });
}

void MainWindow::BTReadData()
{
    char data[1000];
    qint64 len = socket->read((char *)data, 1000);

    QByteArray qa2((char*)data ,len);
//    QString qstr(qa2.toHex());
    qDebug()<<"----" << qa2 << len;
}

void MainWindow::BTSendData(Package package)
{
    switch (package.cmd) {
    case AUDIO:
        socket->write("0010");
        socket->write(package.data);
        break;
    case LED:
        socket->write("0011");
        socket->write(package.data);
        break;
    default:
        break;
    }

    qDebug() << "Send Data";
}

void MainWindow::BTConnected()
{
    this->menuScene->BTConnected();
    qDebug() << "Bluetooth Connected.";
}

void MainWindow::BTUnableConnect(QBluetoothSocket::SocketError err)
{
    if (err == QBluetoothSocket::SocketError::ServiceNotFoundError) {
        this->menuScene->BTConeectionFailed();

        QTimer::singleShot(3000, this, [=](){
            this->menuScene->BTSearching();
            this->discoveryAgent->start();
        });
    }
}

void MainWindow::sendAudioSucess()
{
    Package pkg;
    pkg.cmd = AUDIO;
    pkg.data = QByteArray("00000000000000000000001");
    BTSendData(pkg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

