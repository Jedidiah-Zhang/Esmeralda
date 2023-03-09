#ifndef BUILD_H
#define BUILD_H

#include <QWidget>

class Build : public QWidget
{
    Q_OBJECT
public:
    explicit Build(QWidget *parent = nullptr);

    void setLevel(int lvl);

signals:
    void backButtonClicked();

private:
    int levelIdx;
};

#endif // BUILD_H
