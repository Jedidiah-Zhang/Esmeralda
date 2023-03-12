#ifndef GAMES_H
#define GAMES_H

#include <QWidget>

class Games : public QWidget
{
    Q_OBJECT
public:
    explicit Games(QWidget *parent = nullptr);

signals:
    void backButtonClicked();

};

#endif // GAMES_H
