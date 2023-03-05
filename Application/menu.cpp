#include "menu.h"
#include "pushbutton.h"

#include <QTimer>

Menu::Menu(QWidget *parent)
    : QWidget{parent}
{
    PushButton *levelSelectBtn = new PushButton(QSize(160, 40), ":/resources/images/startButton.png");
    levelSelectBtn->setParent(this);
    // levelSelectBtn->setText("Start");
    levelSelectBtn->move(150, 300);

    connect(levelSelectBtn, &PushButton::clicked, this, [=](){
        levelSelectBtn->bounce(true);
        levelSelectBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->startButtonClicked();
        });
    });
}
