#include "build.h"

#include <QLayout>
#include <QMessageBox>
#include <QDateTime>

Build::Build(QWidget *parent)
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
            if (this->timer->isActive()) {
                pauseTiming();
                QMessageBox msg;
                msg.setText("The building is not finished, and the progress will not be saved.");
                msg.setInformativeText("Do you want to quit?");
                msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                msg.setDefaultButton(QMessageBox::Cancel);
                switch (msg.exec()) {
                    case QMessageBox::Yes:
                        resetTiming();
                        emit this->backButtonClicked();
                        break;
                    case QMessageBox::Cancel:
                        startTiming();
                        break;
                    default:
                        break;
                }
            } else {
                resetTiming();
                emit this->backButtonClicked();
            }
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

void Build::success()
{
    QDateTime curDateTime = QDateTime::currentDateTime();
    int time = curDateTime.toSecsSinceEpoch();
    emit updateRecordFilesReady(this->levelIdx, time, this->timeTaken, this->attempts);

    emit this->backButtonClicked();
    resetTiming();
}

void Build::startTiming()
{
    this->timer->start(10);
    this->tmLbl->setStyleSheet("QLabel{color:#ED6D46;}");
}

void Build::pauseTiming()
{
    this->timer->stop();
    this->tmLbl->setStyleSheet("QLabel{color:black;}");
}

void Build::resetTiming()
{
    this->timer->stop();
    this->timeTaken = 0;
    this->tmLbl->setStyleSheet("QLabel{color:black;}");
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

void Build::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_V) {
        success();
    }
}

