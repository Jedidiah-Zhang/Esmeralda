#include "buildwindow.h"
#include "ui_buildwindow.h"
#include "mainwindow.h"

#include <QPixmap>
#include <QVector>
#include <QImage>
#include <QString>

buildwindow::buildwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::buildwindow)
{
    ui->setupUi(this);

    Buildings = {"Castle1", "Castle2", "Castle3", "House1", "House2", "House3" ,"Building1", "Building2"};
    BuildingStars = {1,2,1,3,1,2,1,0};

    updateStars();
    updateBuildingImages();
}

buildwindow::~buildwindow()
{
    delete ui;
}

void buildwindow::updateBuildingImages(){
    ui->Build1Button->setText(Buildings[0]);
    ui->Build2Button->setText(Buildings[1]);
    ui->Build3Button->setText(Buildings[2]);
    ui->Build4Button->setText(Buildings[3]);
    ui->Build5Button->setText(Buildings[4]);
    ui->Build6Button->setText(Buildings[5]);
    ui->Build7Button->setText(Buildings[6]);
    ui->Build8Button->setText(Buildings[7]);
}

void buildwindow::updateStars(){

    QImage NoStars("C:/Users/anuhg/Documents/Southampton/_Lessons/Year 2/Coursework/D4/QTAPPDESIGN/QTAPPGUI/IMGAssets/NoStar2.png");
    QImage OneStar("C:/Users/anuhg/Documents/Southampton/_Lessons/Year 2/Coursework/D4/QTAPPDESIGN/QTAPPGUI/IMGAssets/oneStar2.png");
    QImage TwoStar("C:/Users/anuhg/Documents/Southampton/_Lessons/Year 2/Coursework/D4/QTAPPDESIGN/QTAPPGUI/IMGAssets/twoStar2.png");
    QImage ThreeStar("C:/Users/anuhg/Documents/Southampton/_Lessons/Year 2/Coursework/D4/QTAPPDESIGN/QTAPPGUI/IMGAssets/threeStar2.png");

    QVector<QImage> Images = {NoStars,OneStar, TwoStar, ThreeStar};

    ui ->Build1Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[0]]));
    ui ->Build2Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[1]]));
    ui ->Build3Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[2]]));
    ui ->Build4Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[3]]));
    ui ->Build5Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[4]]));
    ui ->Build6Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[5]]));
    ui ->Build7Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[6]]));
    ui ->Build8Stars->setPixmap(QPixmap::fromImage(Images[BuildingStars[7]]));

}


void buildwindow::on_Build1Button_pressed()
{

}


void buildwindow::on_Build2Button_pressed()
{

}


void buildwindow::on_Build4Button_pressed()
{

}


void buildwindow::on_Build3Button_pressed()
{

}


void buildwindow::on_Build7Button_pressed()
{

}


void buildwindow::on_Build8Button_pressed()
{

}


void buildwindow::on_Build5Button_pressed()
{

}


void buildwindow::on_Build6Button_pressed()
{

}


void buildwindow::on_Back_pressed()
{
    MainWindow *MAIN = new MainWindow();
    MAIN->show();
    hide();
}

