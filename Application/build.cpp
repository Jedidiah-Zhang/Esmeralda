#include "build.h"
#include "pushbutton.h"


#include <QTimer>

Build::Build(QWidget *parent)
    : QWidget{parent}
{
    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    // link to 3D



    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });

    // keyboard binding
    backBtn->setShortcut(Qt::Key_Escape);
}

void Build::setLevel(int lvl)
{
    this->levelIdx = lvl;
}
