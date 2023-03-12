#ifndef PROGRESS_H
#define PROGRESS_H


#include <QWidget>

class Progress : public QWidget
{
    Q_OBJECT
public:
    explicit Progress(QWidget *parent = nullptr);

signals:
    void backButtonClicked();
};

#endif // PROGRESS_H
