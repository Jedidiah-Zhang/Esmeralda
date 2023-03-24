#ifndef BUILD_H
#define BUILD_H

#include "pushbutton.h"

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>

class Build : public QWidget
{
    Q_OBJECT
public:
    explicit Build(QWidget *parent = nullptr);

    void setLevel(int lvl);
    void startTiming();
    void pauseTiming();
    void resetTiming();

public slots:
    void success();
    void failed();

private slots:
    void timeInc();

signals:
    void backButtonClicked();
    void updateRecordFilesReady(int idx, int curT, int useT, int Att);
    void finishButtonClicked();

private:
    void updateTime();
//    void keyPressEvent(QKeyEvent *event) override;

    int levelIdx = 0;
    int timeTaken = 0;
    int attempts = 1;

    QLabel *tmLbl = nullptr;
    QTimer *timer = nullptr;
    PushButton *startBtn = nullptr;
    PushButton *finishBtn = nullptr;
};

#endif // BUILD_H
