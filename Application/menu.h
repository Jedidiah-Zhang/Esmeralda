#ifndef MENU_H
#define MENU_H

#include <QWidget>

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

signals:
    void startButtonClicked();

};

#endif // MENU_H
