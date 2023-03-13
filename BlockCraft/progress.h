#ifndef PROGRESS_H
#define PROGRESS_H

#include <QWidget>
#include <QChart>
#include <QComboBox>
#include <QLineSeries>
#include <QBarSeries>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QStackedWidget>

class Progress : public QWidget
{
    Q_OBJECT
public:
    explicit Progress(QWidget *parent = nullptr);
//    void setUpIndexes(QVector<int> *idxs);
//    void setSeries(QVector<int> *pts);
    void updateAverage();
    void addChart(int idx, QVector<int> pts);

public slots:
    void changeLevel();

signals:
    void backButtonClicked();
//    void requestLevelData(int lvl);

private:
    QVector<int> levelIndexes;
    QComboBox *selectBox = nullptr;
    QStackedWidget *charts = nullptr;
    QBarSeries *aveSeries = nullptr;
    QBarCategoryAxis *BarAxX = nullptr;
    QValueAxis *BarAxY = nullptr;
    QMap<int, QValueAxis *>AxYs;
    QMap<int, QValueAxis *>AxXs;
    QMap<int, QLineSeries *>lineSeries;
    QMap<int, int> maxTimes;
    QMap<int, double> averages;
};

#endif // PROGRESS_H
