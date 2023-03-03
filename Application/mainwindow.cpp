#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "pushbutton.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(500, 800);

    PushButton *levelSelectBtn = new PushButton(QSize(200, 40), ":/resources/images/yellowButton.png");
    levelSelectBtn->setParent(this);
    // levelSelectBtn->setText("Select Level");

    levelSelectScene = new levelSelect;


    connect(levelSelectBtn, &PushButton::clicked, this, [=](){
        levelSelectBtn->bounce(true);
        levelSelectBtn->bounce(false);

        QTimer::singleShot(200, this, [=](){
            this->hide();
            levelSelectScene->show();
        });
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

