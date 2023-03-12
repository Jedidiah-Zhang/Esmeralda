#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QToolButton>

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

    void determineGeometry();

public slots:
    void shiftLeft();
    void shiftRight();

signals:
    void centralBtnClicked();
    void leftBtnClicked(int);
    void rightBtnClicked(int);

private:
    int W_WIDTH, W_HEIGHT, currentBtnIdx;
    QWidget *parent;
    const static QVector<QString> menuOptions;
    QToolButton *CentralBtn = nullptr;
    QToolButton *leftBtn = nullptr;
    QToolButton *rightBtn = nullptr;

};

#endif // MENU_H
