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

    this->model = new Open3DModel;

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

    /*==========FileReading==========*/
    // load levels
    this->levels = new QMap<int, QVector<Block> *>;
    QDir dirLevel("./levels/");
    dirLevel.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList lvlList = dirLevel.entryInfoList();
    for (int i = 0; i < lvlList.count(); i++) {
        QFileInfo info = lvlList.at(i);
        QFile file(info.absoluteFilePath());
        readLevelFile(info.absoluteFilePath(), info.baseName());
    }
    this->levelSelectScene->setLevels(this->levels->count());
//    this->levelSelectScene->setLevels(2);

    // load records
    this->records = new QMap<int, QVector<Record> *>;
    QDir dirRecord("./records/");
    dirRecord.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList rcdList = dirRecord.entryInfoList();
    // go through every file
    for(int i = 0; i < rcdList.count(); i++) {
        QFileInfo info = rcdList.at(i);
        QFile file(info.absoluteFilePath());
        readRecordFile(info.absoluteFilePath(), info.baseName());
    }

    /*==========BlueTooth==========*/
    this->blueTooth = new BlueTooth(this);
    connect(blueTooth, &BlueTooth::EndSearch, menuScene, &Menu::BTSearchFailed);
    connect(blueTooth, &BlueTooth::Searching, menuScene, &Menu::BTSearching);
    connect(blueTooth, &BlueTooth::Connecting, menuScene, &Menu::BTConnecting);
    connect(blueTooth, &BlueTooth::Connected, menuScene, &Menu::BTConnected);
    connect(blueTooth, &BlueTooth::ConncetFaild, menuScene, &Menu::BTConeectionFailed);
    connect(blueTooth, &BlueTooth::ConncetLost, menuScene, &Menu::BTConnectionLost);
    connect(blueTooth, &BlueTooth::ComfirmTrue, buildScene, &Build::success);
    connect(blueTooth, &BlueTooth::ComfirmFalse, buildScene, &Build::failed);

    this->blueTooth->startSearch();

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
    connect(this->buildScene, &Build::finishButtonClicked, blueTooth, &BlueTooth::sendCompletion);
    connect(this->buildScene, &Build::updateRecordFilesReady, this, &MainWindow::updateRecordFiles);
    connect(this->buildScene, &Build::updateRecordFilesReady, blueTooth, &BlueTooth::sendAudioSucess);

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
    this->buildScene->setLevel(lvl);
    this->menuList->setCurrentIndex(BUILD);
    this->model->run();
    this->blueTooth->sendStructure(this->levels->value(lvl));
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
            block.tile = blockObj.value("tile").toInt();
            block.height = blockObj.value("height").toInt();
            block.ID = blockObj.value("ID").toInt();
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
    QDir dir = QDir("./records/");
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
    this->model->kill();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

