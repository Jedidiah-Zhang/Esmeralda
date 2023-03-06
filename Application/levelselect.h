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
    void resizeWidgets();
    void determineGeometry();

signals:
    void backButtonClicked();

private:
    int W_WIDTH, W_HEIGHT, columns;
    QWidget *parent;
    QScrollArea *scrollArea = nullptr;
    QFrame *selectGrid = nullptr;
    QGridLayout *gridLayout = nullptr;
    QVector<PushButton *> buttons;
};

#endif // LEVELSELECT_H
