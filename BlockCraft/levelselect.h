#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include "pushbutton.h"

#include <QWidget>
#include <QLayout>
#include <QFrame>
#include <QScrollArea>
#include <QVector>

class levelSelect : public QWidget
{
    Q_OBJECT
public:
    levelSelect(QWidget *parent = nullptr);
    void determineGeometry();

public slots:
    void setLevels(int levelCount);
    void setStarRecords(int idx, int stars);

signals:
    void backButtonClicked();
    void levelSelected(int);

private:
    int W_WIDTH, W_HEIGHT;
    int columns = 0, levelcount = 0;
    QWidget *parent;
    QScrollArea *scrollArea = nullptr;
    QFrame *selectGrid = nullptr;
    QGridLayout *gridLayout = nullptr;
    QVector<PushButton *> buttons;
    QVector<QWidget *> stars;
    QVector<int> starRecords;
};

#endif // LEVELSELECT_H
