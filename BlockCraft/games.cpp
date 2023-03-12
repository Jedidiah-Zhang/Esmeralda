#include "games.h"
#include "pushbutton.h"

#include <QTimer>

Games::Games(QWidget *parent)
    : QWidget{parent}
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, QColor(224, 223, 198)); // #E0DFC6
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    // send signal as back button been clicked
    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });
}

