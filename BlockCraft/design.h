#ifndef DESIGN_H
#define DESIGN_H

#include <QWidget>

class Design : public QWidget
{
    Q_OBJECT
public:
    explicit Design(QWidget *parent = nullptr);

signals:
    void backButtonClicked();

};

#endif // DESIGN_H
