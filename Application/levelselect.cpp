#include "levelselect.h"
#include "pushbutton.h"

#include <QTimer>
#include <QLayout>
#include <QFrame>
#include <QScrollArea>

levelSelect::levelSelect(QWidget *parent)
    : QWidget(parent)
{
//    this->setFixedSize(500, 800);
//    this->setWindowTitle("Select Level");
    this->W_WIDTH = parent->size().width();
    this->W_HEIGHT = parent->size().height();
    int columns = 2;

    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    QScrollArea *scrollArea = new QScrollArea(this);
    QFrame *selectGrid = new QFrame(this);
    QGridLayout *gridLayout = new QGridLayout(this);

    for (int i = 0; i < 3; i++) {
        PushButton *button = new PushButton(QSize(200, 200), "://resources/images/defaultLevelBackground.png");
        gridLayout->addWidget(button, i/columns, i%columns, 1, 1);

        connect(button, &PushButton::clicked, this, [=](){
            qDebug() << "i = " << i;
        });
    }

    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setHorizontalSpacing((W_WIDTH-30-200*columns)/3);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setContentsMargins((W_WIDTH-30-200*columns)/3, 10, (W_WIDTH-30-200*columns)/3, 10);

    selectGrid->setLayout(gridLayout);
    // selectGrid->setFrameShape(QFrame::Box);

    scrollArea->setWidget(selectGrid);
    scrollArea->move(5, 40);
    scrollArea->setFixedSize(W_WIDTH-10, W_HEIGHT-50);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // send signal as back button been clicked
    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });
}
