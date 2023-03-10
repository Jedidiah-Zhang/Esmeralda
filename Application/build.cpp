#include "build.h"

#include <QLayout>


Build::Build(QWidget *parent)
    : QWidget{parent}
{
    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    // timer
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->timer = new QTimer(this);
    this->tmLbl = new QLabel(this);
    this->tmLbl->setFont(QFont("Times New Roman", 30));
    this->tmLbl->setText("00 : 00 : 00");
    layout->addWidget(tmLbl, 0, Qt::AlignHCenter);

    connect(this->timer, &QTimer::timeout, this, &Build::timeInc);

    //start button
    this->startBtn = new PushButton(QSize(220, 80), "://resources/images/startButton.png");
    startBtn->setParent(this);
    layout->addWidget(startBtn, 0, Qt::AlignHCenter);

    layout->setAlignment(Qt::AlignCenter);

    // link to 3D



    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
            resetTiming();
        });
    });

    connect(startBtn, &PushButton::clicked, this, [=](){
        startBtn->bounce(true);
        startBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            // hides the start button after clicking
            startBtn->hide();
            this->startTiming();
        });
    });

    // keyboard binding
    backBtn->setShortcut(Qt::Key_Escape);
}

void Build::setLevel(int lvl)
{
    this->levelIdx = lvl;
}

void Build::startTiming()
{
    this->timer->start(10);
}

void Build::pauseTiming()
{
    this->timer->stop();
}
void Build::resetTiming()
{
    this->timer->stop();
    this->timeTaken = 0;
    this->startBtn->show();
    updateTime();
}

void Build::timeInc()
{
    this->timeTaken++;
    updateTime();
}

void Build::updateTime()
{
    int hour, min, sec, mil;
    mil = this->timeTaken % 100;
    sec = (this->timeTaken / 100) % 60;
    min = (this->timeTaken / 100 / 60) % 60;
    hour = (this->timeTaken / 100 / 60 / 60) % 60;
    QString time = QString("%1 : %2 : %3 : %4") \
                   .arg(hour, 2, 10, QLatin1Char('0')) \
                   .arg(min, 2, 10, QLatin1Char('0')) \
                   .arg(sec, 2, 10, QLatin1Char('0')) \
                   .arg(mil, 2, 10, QLatin1Char('0'));
    this->tmLbl->setText(time);
}

