#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVector>
#include <QString>


#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector<QString> MenuOptions; //QVector holding String names of specific windows
    int MenuOptionsPtr; //Pointer that holds current window user wants to choose.
    enum MenuOptionsLiteral{Build, Create, Games, Progress};//enumerator for window names

    void updateButtonNames();


public slots:
    void openSpecificWindow(); //slots control button input, Open specfic window triggers the launching of the next window
    void shiftRightMenuOptions(); //shifts the menu options to the right,
    void shiftLeftMenuOptions(); //shifts menu options to the left.

private slots:

    void on_MenuOptionButton_pressed();
    void on_NextOptionButton_pressed();
    void on_PrevOptionButton_pressed();

    void on_MenuOptionButton_clicked();
};
#endif // MAINWINDOW_H
