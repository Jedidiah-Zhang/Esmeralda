#include "pushbutton.h"
#include <QPropertyAnimation>

PushButton::PushButton(QSize size, QString normalImg, QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if (!ret){
        qDebug() << "Image loading failed";
        return;
    }

    this->setFixedSize(size.width(), size.height());
    // make the button transparent outside the icon.
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(size);

}

void PushButton::bounce(bool direction)
{
    // animation for button been clicked,
    // direction determines whether the button moves up or down.
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(200);
    if (direction) {
        animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
        animation->setEndValue(QRect(this->x(), this->y()+3, this->width(), this->height()));
    } else {
        animation->setStartValue(QRect(this->x(), this->y()+3, this->width(), this->height()));
        animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
    }
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

