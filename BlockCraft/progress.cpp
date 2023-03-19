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

    chart->addSeries(this->lineSeries[idx]);
    chart->addAxis(AxXs[idx], Qt::AlignBottom);
    chart->addAxis(AxYs[idx], Qt::AlignLeft);
    this->lineSeries[idx]->attachAxis(AxXs[idx]);
    this->lineSeries[idx]->attachAxis(AxYs[idx]);
    this->lineSeries[idx]->setPointsVisible(true);
    AxXs[idx]->setTitleText("Records");
    AxYs[idx]->setTitleText("Time Used (Minutes)");

    int max = 0, sum = 0;
    for (int i = 0; i < pts.length(); i++) {
        max = pts[i] > max ? pts[i] : max;
        sum += pts[i];
        this->lineSeries[idx]->append(i, (double)pts[i]/6000);
    }
    maxTimes.insert(idx, max);
    averages.insert(idx, (double)sum/pts.length());
    dataLength.insert(idx, pts.length());
    double maxMin = (double) max / 6000;
    AxXs[idx]->setRange(0, pts.length());
    AxXs[idx]->setTickCount(pts.length()+1);
    AxYs[idx]->setRange(0, maxMin+1);
    AxYs[idx]->setTickCount(11);

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
        max = averages[levelIndexes[i]] > max ? averages[levelIndexes[i]] : max;
        *timeBar << this->averages[levelIndexes[i]] / 6000;
        this->BarAxX->append(QString("Level %1").arg(this->levelIndexes[i]+1));
    }
    double maxMin = (double) max / 6000;
    this->BarAxY->setRange(0, maxMin+1);
    this->BarAxY->setTickCount(11);
    this->aveSeries->clear();
    this->aveSeries->append(timeBar);
}

void Progress::addPoint(int idx, int pt)
{
    qDebug() << idx << pt;
    this->lineSeries.value(idx)->append(dataLength[idx], pt);
    if (pt > this->maxTimes[idx]) {
        this->maxTimes[idx] =  pt;
        double maxMin = (double) pt / 6000;
        AxYs.value(idx)->setRange(0, maxMin+1);
    }
    dataLength[idx]++;
    averages[idx] = (double) maxTimes[idx] / dataLength[idx];
    AxXs[idx]->setRange(0, dataLength[idx]);
    AxXs[idx]->setTickCount(dataLength[idx]+1);

    updateAverage();
}

void Progress::changeLevel()
{
    this->charts->setCurrentIndex(this->selectBox->currentIndex());
}

