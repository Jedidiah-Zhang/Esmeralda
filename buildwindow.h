#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QImage>

namespace Ui {
class buildwindow;
}

class buildwindow : public QWidget
{
    Q_OBJECT

public:
    explicit buildwindow(QWidget *parent = nullptr);
    ~buildwindow();

private slots:
    void on_Back_pressed();

    void on_Build1Button_pressed();

    void on_Build2Button_pressed();

    void on_Build3Button_pressed();

    void on_Build4Button_pressed();

    void on_Build5Button_pressed();

    void on_Build6Button_pressed();

    void on_Build7Button_pressed();

    void on_Build8Button_pressed();



private:
    Ui::buildwindow *ui;

    enum BuildingsLiteral {Castle1, Castle2, Castle3, House1, House2, Building3};
    QVector<QString> Buildings;
    QVector<int> BuildingStars;

    void updateStars();

    void updateBuildingImages();

    QVector<QImage> Images;
};

#endif // BUILDWINDOW_H
