#include "progress.h"
#include "pushbutton.h"

#include <QTimer>
#include <QLayout>
#include <QChartView>
#include <QBarSet>

Progress::Progress(QWidget *parent)
    : QWidget{parent}
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, QColor(224, 223, 198)); // #E0DFC6
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->selectBox = new QComboBox(this);
    this->charts = new QStackedWidget(this);

    selectBox->addItem("AVERAGE");
    selectBox->setFixedSize(220, 30);
    selectBox->setFont(QFont("Arial", 14));
    selectBox->setStyleSheet("QComboBox{"
                             "border-radius:5px;"
                             "border:1px solid grey;"
                             "background-color:#F3ECE0;"
                             "}");
    layout->addWidget(selectBox, 0, Qt::AlignHCenter);
    layout->addWidget(charts);

    QChartView *averageView = new QChartView(this);
    QChart *averageChart = new QChart;
    this->aveSeries = new QBarSeries;
    this->BarAxX = new QBarCategoryAxis;
    this->BarAxY = new QValueAxis;

    averageChart->setTheme(QChart::ChartThemeBrownSand);
    averageChart->setContentsMargins(0, 0, 0, 0);
    averageChart->setBackgroundRoundness(30);
    averageChart->legend()->hide();

    averageChart->addSeries(aveSeries);
    averageChart->addAxis(BarAxX, Qt::AlignBottom);
    averageChart->addAxis(BarAxY, Qt::AlignLeft);
    this->aveSeries->attachAxis(BarAxX);
    this->aveSeries->attachAxis(BarAxY);
    this->BarAxY->setTitleText("Time Used (Minutes)");

    averageView->setChart(averageChart);
    this->charts->addWidget(averageView);

    // send signal as back button been clicked
    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });

    connect(this->selectBox, &QComboBox::currentIndexChanged, this, &Progress::changeLevel);

}

void Progress::addChart(int idx, QVector<int> pts)
{
    this->levelIndexes.push_back(idx);
    this->selectBox->addItem(QString("LEVEL %1").arg(idx+1));
    QChartView *chartView = new QChartView(this);
    QChart *chart = new QChart;
    this->lineSeries.insert(idx, new QLineSeries);
    this->AxXs.insert(idx, new QValueAxis);
    this->AxYs.insert(idx, new QValueAxis);

    chart->setTheme(QChart::ChartThemeBrownSand);
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(30);
    chart->legend()->hide();

    chart->addSeries(this->lineSeries.value(idx));
    chart->addAxis(AxXs.value(idx), Qt::AlignBottom);
    chart->addAxis(AxYs.value(idx), Qt::AlignLeft);
    this->lineSeries.value(idx)->attachAxis(AxXs.value(idx));
    this->lineSeries.value(idx)->attachAxis(AxYs.value(idx));
    this->lineSeries.value(idx)->setPointsVisible(true);
    AxXs.value(idx)->setTitleText("Records");
    AxYs.value(idx)->setTitleText("Time Used (Minutes)");

    int max = 0;
    for (int i = 0; i < pts.length(); i++) {
        max = pts.at(i) > max ? pts.at(i) : max;
        this->lineSeries.value(idx)->append(i, (double)pts.at(i)/6000);
    }
    maxTimes.insert(idx, max);
    averages.insert(idx, (double)max/pts.length());
    double maxMin = (double) max / 6000;
    AxXs.value(idx)->setRange(0, pts.length());
    AxXs.value(idx)->setTickCount(pts.length()+1);
    AxYs.value(idx)->setRange(0, maxMin+1);
    AxYs.value(idx)->setTickCount(11);

    chartView->setChart(chart);
    this->charts->addWidget(chartView);

    updateAverage();
}

void Progress::updateAverage()
{
    int max = 0;
    QBarSet *timeBar = new QBarSet("Time Used");
    this->BarAxX->clear();
    for (int i = 0; i < this->levelIndexes.length(); i++)
    {
        max = averages.value(levelIndexes.at(i)) > max ? averages.value(levelIndexes.at(i)) : max;
        *timeBar << this->averages.value(levelIndexes.at(i))/6000;
        this->BarAxX->append(QString("Level %1").arg(this->levelIndexes.at(i)+1));
    }
    double maxMin = (double) max / 6000;
    this->BarAxY->setRange(0, maxMin+1);
    this->BarAxY->setTickCount(11);
    this->aveSeries->clear();
    this->aveSeries->append(timeBar);
}

//void Progress::setUpIndexes(QVector<int> *idxs)
//{
//    this->levelIndexes = idxs;
//    for (int i = 0; i < idxs->length(); i++) {
//        this->selectBox->addItem(QString("LEVEL %1").arg(idxs->at(i)+1));
//        QLineSeries *lSeries = new QLineSeries();
//        QValueAxis *AxX = new QValueAxis();
//        QValueAxis *AxY = new QValueAxis();
//        AxX->setTitleText("Records");
//        AxY->setTitleText("Time Used (Minutes)");
//        this->AxXs.push_back(AxX);
//        this->AxYs.push_back(AxY);
//        this->lineSeries.push_back(lSeries);
//        this->chart->addSeries(lSeries);
//        this->chart->addAxis(AxX, Qt::AlignBottom);
//        this->chart->addAxis(AxY, Qt::AlignLeft);
//        lSeries->setVisible(false);
//        emit requestLevelData(this->levelIndexes->at(i));
//    }
//}

//void Progress::setSeries(QVector<int> *pts)
//{
//    this->maxTimes.append(0);
//    for (int i = 0; i < pts->length(); i++) {
//        maxTimes.back() = pts->at(i) > maxTimes.back() ? pts->at(i) : maxTimes.back();
//        this->lineSeries.back()->append(i, (double)pts->at(i)/6000);

//    }
//    double maxMin = (double) maxTimes.back() / 6000;
//    this->AxYs.back()->setRange(0, maxMin+1);
//    this->AxYs.back()->setTickCount(11);
//    this->AxXs.back()->setRange(1, this->lineSeries.back()->count());
//    this->AxXs.back()->setTickCount(this->lineSeries.back()->count());
//}

//void Progress::setAverage(QVector<double> *pts)
//{
//    if (this->barSeries == nullptr || pts != nullptr) {
//        QBarSet *timeBar = new QBarSet("Time Used");
////    QBarSet *attBar = new QBarSet("Attempts");
//        for (int i = 0; i < pts->length(); i++) {
//            *timeBar << pts->at(i)/6000;
//        }
//        this->barSeries->append(timeBar);

//        int maxTime = 0;
//        for (int i = 0; i < this->levelIndexes->length(); i++) {
//            maxTime = pts->at(i) > maxTime ? pts->at(i) : maxTime;
//            BarAxX->append(QString("Level %1").arg(this->levelIndexes->at(i)+1));
//        }
//        double maxMin = (double) maxTime / 6000;
//        this->BarAxY->setRange(0, maxMin+1);
//        this->BarAxY->setTickCount(11);
//        this->chart->addSeries(this->barSeries);
//        this->barSeries->attachAxis(BarAxX);
//        this->barSeries->attachAxis(BarAxY);
//    }

////    this->chart->removeAllSeries();
//    foreach (QAbstractSeries *series, chart->series()) {
//        if (dynamic_cast<QLineSeries*>(series)) {
//            QLineSeries *lineSer = dynamic_cast<QLineSeries*>(series);
//            lineSer->setVisible(false);
//        }
//        else if (dynamic_cast<QBarSeries*>(series)) {
//            QBarSeries *barSer = dynamic_cast<QBarSeries*>(series);
//            barSer->setVisible(true);
//        }
//    }
//    for (int i = 0; i < this->AxXs.length(); i++) {
//        this->chart->removeAxis(AxXs.at(i));
//        this->chart->removeAxis(AxYs.at(i));
//    }
//    this->chart->addAxis(this->BarAxX, Qt::AlignBottom);
//    this->chart->addAxis(this->BarAxY, Qt::AlignLeft);
//}

void Progress::changeLevel()
{
    this->charts->setCurrentIndex(this->selectBox->currentIndex());
}

