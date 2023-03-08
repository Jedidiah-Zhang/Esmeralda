#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setFixedSize(500, 800);

    // show only one scene at a time.
    this->menuList = new QStackedWidget(this);

    //note that the order of scenes added to menuList is the same as the enum
    this->levelSelectScene = new levelSelect(this);
    this->minigameScene = new QWidget(this);
    this->designScene = new QWidget(this);
    this->progressScene = new QWidget(this);
    this->menuScene = new Menu(this);
    this->buildScene = new QWidget(this);

    this->menuList->addWidget(this->levelSelectScene);
    this->menuList->addWidget(this->minigameScene);
    this->menuList->addWidget(this->designScene);
    this->menuList->addWidget(this->progressScene);
    this->menuList->addWidget(this->menuScene);
    this->menuList->addWidget(this->buildScene);

    setCentralWidget(this->menuList);
    this->menuList->setCurrentIndex(MENU);

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
}

void MainWindow::changeScene(int idx){
//    qDebug() << idx;
    this->menuScene->disconnect(SIGNAL(centralBtnClicked()));
    connect(this->menuScene, &Menu::centralBtnClicked, this, [=](){
        this->menuList->setCurrentIndex(SCENES(idx));
    });
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    this->menuScene->determineGeometry();
    this->levelSelectScene->determineGeometry();
}

MainWindow::~MainWindow()
{
    delete ui;
}

