#include "mainwindow.h"
#include "./ui_mainwindow.h"

enum {MENU, LEVELSELECT};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // this->setFixedSize(500, 800);

    this->menuList = new QStackedWidget(this);

    this->menuScene = new Menu(this);
    this->levelSelectScene = new levelSelect(this);

    this->menuList->addWidget(menuScene);
    this->menuList->addWidget(levelSelectScene);

    setCentralWidget(this->menuList);

    connect(this->menuScene, &Menu::startButtonClicked, this, [=](){
        // qDebug() << this->menuList->currentIndex();
        this->menuList->setCurrentIndex(LEVELSELECT);
    });
    connect(this->levelSelectScene, &levelSelect::backButtonClicked, this, [=](){
        this->menuList->setCurrentIndex(MENU);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

