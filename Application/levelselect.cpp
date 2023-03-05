#include "levelselect.h"
#include "pushbutton.h"

#include <QTimer>

levelSelect::levelSelect(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(500, 800);
    this->setWindowTitle("Select Level");

    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);

    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });
}
