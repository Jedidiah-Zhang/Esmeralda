#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <QWidget>

class levelSelect : public QWidget
{
    Q_OBJECT
public:
    levelSelect(QWidget *parent = nullptr);

signals:
    void backButtonClicked();
};

#endif // LEVELSELECT_H
