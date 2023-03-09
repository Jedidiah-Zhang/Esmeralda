#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menu.h"
#include "levelselect.h"
#include "build.h"
#include "design.h"
#include "games.h"
#include "progress.h"

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
    void toBuildScene(int lvl);

signals:

private:
    virtual void resizeEvent(QResizeEvent *) override;
    Ui::MainWindow *ui;

    QStackedWidget *menuList = nullptr;
    Menu *menuScene = nullptr;
    levelSelect *levelSelectScene = nullptr;
    Games *minigameScene = nullptr;
    Design *designScene = nullptr;
    Progress *progressScene = nullptr;
    Build *buildScene = nullptr;
};
#endif // MAINWINDOW_H
