#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structs.h"
#include "open3dmodel.h"
#include "menu.h"
#include "levelselect.h"
#include "build.h"
#include "design.h"
#include "games.h"
#include "progress.h"
#include "bluetooth.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void readRecordFile(QString fileDir, QString level);
    void readLevelFile(QString fileDir, QString level);

    ~MainWindow();

public slots:
    void changeScene(int idx);
    void toBuildScene(int lvl);
    void updateRecordFiles(int idx, int curT, int useT, int Att);

private slots:

signals:

private:
    virtual void resizeEvent(QResizeEvent *) override;

    Ui::MainWindow *ui;

    Open3DModel *model = nullptr;

    QStackedWidget *menuList = nullptr;
    Menu *menuScene = nullptr;
    levelSelect *levelSelectScene = nullptr;
    Games *minigameScene = nullptr;
    Design *designScene = nullptr;
    Progress *progressScene = nullptr;
    Build *buildScene = nullptr;

    QMap<int, QVector<Record> *> *records = nullptr;
    QMap<int, QVector<Block> *> *levels = nullptr;

    BlueTooth *blueTooth = nullptr;

};
#endif // MAINWINDOW_H
