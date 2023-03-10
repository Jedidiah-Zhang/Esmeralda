#ifndef BUILD_H
#define BUILD_H

#include "pushbutton.h"

#include <QWidget>
#include <QLabel>
#include <QTimer>

class Build : public QWidget
{
    Q_OBJECT
public:
    explicit Build(QWidget *parent = nullptr);

    void setLevel(int lvl);
    void startTiming();
    void pauseTiming();
    void resetTiming();

signals:
    void backButtonClicked();

public slots:
    void timeInc();

private:
    void updateTime();

    int levelIdx = 0;
    int timeTaken = 0;

    QLabel *tmLbl = nullptr;
    QTimer *timer = nullptr;
    PushButton *startBtn = nullptr;
};

#endif // BUILD_H
