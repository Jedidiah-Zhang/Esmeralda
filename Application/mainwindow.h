#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menu.h"
#include "levelselect.h"

#include <QMainWindow>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum SCENES{
    LEVELSELECT = 0,
    GAME,
    DESIGN,
    PROGRESS,
    MENU,
    BUILD
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void changeScene(int idx);

signals:

private:
    virtual void resizeEvent(QResizeEvent *) override;
    Ui::MainWindow *ui;

    QStackedWidget *menuList = nullptr;
    Menu *menuScene = nullptr;
    levelSelect *levelSelectScene = nullptr;
    QWidget *minigameScene = nullptr;
    QWidget *designScene = nullptr;
    QWidget *progressScene = nullptr;
    QWidget *buildScene = nullptr;
};
#endif // MAINWINDOW_H
