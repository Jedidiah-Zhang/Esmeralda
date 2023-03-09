#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

MainWindow::~MainWindow()
{
    delete ui;
}

