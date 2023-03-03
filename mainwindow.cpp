#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

#include "buildwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MenuOptionsPtr = 0;

    MenuOptions = {"Build", "Create", "Games", "Progress"};

    updateButtonNames();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSpecificWindow(){

    buildwindow *BUILD = new buildwindow();

    switch((MenuOptionsLiteral)MenuOptionsPtr){

        case Build:
            qDebug() << "Build";
            BUILD->show();
            hide();
            break;

        case Create:
            qDebug() << "Create";
            break;

        case Games:
            qDebug() << "Games";
            break;

        case Progress:
            qDebug() << "Progress";
            break;


    }


}

void MainWindow::updateButtonNames(){//update button names

    int PrevPtr = 0;
    int NextPtr = 0;

    if(MenuOptionsPtr-1<0){ //adjust pointers to allow circular menu
        PrevPtr = MenuOptions.length()-1;
    }else{
        PrevPtr = MenuOptionsPtr-1;
    }

    if(MenuOptionsPtr+1 >= MenuOptions.length()){
        NextPtr = 0;
    }else{
        NextPtr = MenuOptionsPtr+1;
    }

    ui -> PrevOptionButton->setText(MenuOptions[PrevPtr]); //change the button names
    ui -> MenuOptionButton->setText(MenuOptions[MenuOptionsPtr]);
    ui -> NextOptionButton->setText(MenuOptions[NextPtr]);
}

void MainWindow::shiftRightMenuOptions(){
    if(MenuOptionsPtr == MenuOptions.length()-1){
        MenuOptionsPtr = 0;
    }else{
        MenuOptionsPtr = MenuOptionsPtr + 1;
    }
    updateButtonNames();
}

void MainWindow::shiftLeftMenuOptions(){
    if(MenuOptionsPtr == 0){
        MenuOptionsPtr = MenuOptions.length()-1;
    }else{
        MenuOptionsPtr = MenuOptionsPtr - 1;
    }
    updateButtonNames();
}


void MainWindow::on_MenuOptionButton_pressed(){
    openSpecificWindow();
}

void MainWindow::on_NextOptionButton_pressed(){
    shiftRightMenuOptions();
}

void MainWindow::on_PrevOptionButton_pressed(){
    shiftLeftMenuOptions();
}

void MainWindow::on_MenuOptionButton_clicked(){

}


