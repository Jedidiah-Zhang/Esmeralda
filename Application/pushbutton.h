#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    PushButton(QSize size, QString normalImg, QString pressImg = "");

    void bounce(bool dirction);

protected:
    QString normalImgPath;
    QString pressImgPath;

signals:


};

#endif // PUSHBUTTON_H
