#include "menu.h"

#include <QTimer>

// these options should be the same order as enum SCENES
// otherwise the index won't match
const QVector<QString> Menu::menuOptions = {"START", "MINIGAMES", "DESIGN", "PROGRESS"};

Menu::Menu(QWidget *parent)
    : QWidget{parent}
{
    this->parent = parent;
    this->W_WIDTH = parent->width();
    this->W_HEIGHT = parent->height();
    this->currentBtnIdx = 0;

    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, QColor(224, 223, 198)); // #E0DFC6
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->CentralBtn = new QToolButton(this);
    this->CentralBtn->setStyleSheet("QToolButton{"
                                    "background-color:#ED6D46;"
                                    "color:#E0DFC6;"
                                    "font:bold;"
                                    "border-radius:20px;"
                                    "border:2px groove gray;"
                                    "border-style:outset;"
                                    "}");
    this->CentralBtn->setFont(QFont("Times New Roman", 28));
    this->CentralBtn->setText(menuOptions[currentBtnIdx]);

    this->leftBtn = new QToolButton(this);
    this->leftBtn->setFont(QFont("Times New Roman", 20));
    this->leftBtn->setText("PREV");

    this->rightBtn = new QToolButton(this);
    this->rightBtn->setFont(QFont("Times New Roman", 20));
    this->rightBtn->setText("NEXT");

    connect(CentralBtn, &QToolButton::clicked, this, &Menu::centralBtnClicked);

    connect(this->leftBtn, &QToolButton::clicked, this, &Menu::shiftLeft);
    connect(this->rightBtn, &QToolButton::clicked, this, &Menu::shiftRight);

    // keyboard binding
    this->CentralBtn->setShortcut(Qt::Key_Return);
    this->leftBtn->setShortcut(Qt::Key_Left);
    this->rightBtn->setShortcut(Qt::Key_Right);

    determineGeometry();
}

void Menu::determineGeometry()
{
    this->W_WIDTH = this->parent->width();
    this->W_HEIGHT = this->parent->height();

    this->CentralBtn->setFixedSize(W_WIDTH/2, W_HEIGHT/2);
    this->CentralBtn->move(W_WIDTH/4, W_HEIGHT/4);

    this->leftBtn->setFixedSize(W_WIDTH/4, W_HEIGHT/3);
    this->leftBtn->move(-W_WIDTH/8, W_HEIGHT/3);
    this->leftBtn->setStyleSheet(QString("QToolButton{"
                                         "background-color:#ED6D46;"
                                         "color:#E0DFC6;"
                                         "border-radius:20px;"
                                         "border:2px groove gray;"
                                         "border-style:outset;"
                                         "padding-left: %1"
                                         "}").arg(W_WIDTH/8));

    this->rightBtn->setFixedSize(W_WIDTH/4, W_HEIGHT/3);
    this->rightBtn->move(7*W_WIDTH/8, W_HEIGHT/3);
    this->rightBtn->setStyleSheet(QString("QToolButton{"
                                          "background-color:#ED6D46;"
                                          "color:#E0DFC6;"
                                          "border-radius:20px;"
                                          "border:2px groove gray;"
                                          "border-style:outset;"
                                          "padding-right: %1"
                                          "}").arg(W_WIDTH/8));
}

void Menu::shiftLeft()
{
    if (this->currentBtnIdx > 0) {
        this->currentBtnIdx--;
    } else {
        this->currentBtnIdx = menuOptions.length()-1;
    }
    this->CentralBtn->setText(menuOptions[this->currentBtnIdx]);
    emit leftBtnClicked(currentBtnIdx);
}

void Menu::shiftRight()
{
    if (this->currentBtnIdx < menuOptions.length()-1) {
        this->currentBtnIdx++;
    } else {
        this->currentBtnIdx = 0;
    }
    this->CentralBtn->setText(menuOptions[this->currentBtnIdx]);
    emit rightBtnClicked(currentBtnIdx);
}
