#include "levelselect.h"

#include <QTimer>
#include <QImage>
#include <QLabel>

levelSelect::levelSelect(QWidget *parent)
    : QWidget(parent)
{
    this->parent = parent;
    this->W_WIDTH = parent->size().width();

    this->levelcount = 15;
    for (int i = 0; i < levelcount; i++) {
        this->starRecords.push_back(4);
    }

    this->scrollArea = new QScrollArea(this);
    this->selectGrid = new QFrame(this->scrollArea);
    this->gridLayout = new QGridLayout(this->selectGrid);

    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    // create buttons
    for (int i = 0; i < levelcount; i++) {
        PushButton *button = new PushButton(QSize(200, 200), "://resources/images/defaultLevelBackground.png");
        button->setParent(this->scrollArea);
        button->setStyleSheet("QPushButton{background-color:gray;"
                              "color:white;"
                              "border-radius:10px;"
                              "border:2px groove gray;"
                              "border-style:outset;}");
        this->buttons.push_back(button);

        connect(button, &PushButton::clicked, this, [=](){
//            qDebug() << "i = " << i;
            button->bounce(true);
            button->bounce(false);

            QTimer::singleShot(100, this, [=](){
                emit this->levelSelected(i);
            });
        });
    }

    //star labels
    for (int i = 0; i < levelcount; i++) {
        QWidget *star = new QWidget(this->scrollArea);
        //
        star->setFixedSize(130, 50);
        star->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        this->stars.push_back(star);
    }

    // send signal as back button been clicked
    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });

    // keyboard binding
    backBtn->setShortcut(Qt::Key_Escape);

    determineGeometry();
}

void levelSelect::determineGeometry()
{
    this->W_WIDTH = this->parent->size().width();
    this->W_HEIGHT = this->parent->size().height();
    // determine how many columns able to display in terms of the width of the window.
    this->columns = (W_WIDTH-20)/220;

    // rearrange the buttons and stars
    for (int i = 0; i < this->buttons.length(); i++) {
        gridLayout->addWidget(this->buttons[i], i/columns, i%columns, 1, 1, Qt::AlignCenter);
        gridLayout->addWidget(this->stars[i], i/columns, i%columns, 1, 1, Qt::AlignBottom | Qt::AlignRight);
//        if (this->starRecords[i] == 4) this->stars[i]->hide();
    }

    // change the spacing as the window size changed
    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    gridLayout->setHorizontalSpacing((W_WIDTH-30-200*columns)/(columns+1));
    gridLayout->setVerticalSpacing(10);
    gridLayout->setContentsMargins((W_WIDTH-30-200*columns)/(columns+1), 10,
                                   (W_WIDTH-30-200*columns)/(columns+1), 10);

    selectGrid->setLayout(gridLayout);
    selectGrid->setFixedSize(W_WIDTH-30, (buttons.length()+columns-1)/columns*210+20);

    scrollArea->setWidget(selectGrid);
    scrollArea->move(5, 40);
//    qDebug() << W_WIDTH << W_HEIGHT;
    scrollArea->setFixedSize(W_WIDTH-10, W_HEIGHT-50);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // set background in the selection area
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QImage("://resources/images/Bricks.png").scaled(64, 64)));
    scrollArea->setAutoFillBackground(true);
    scrollArea->setPalette(pal);
}

void levelSelect::setStarRecords(int idx, int time)
{
    if (time > 60000) this->starRecords[idx] = 0;
    else if (time <= 60000 && time > 30000) this->starRecords[idx] = 1;
    else if (time <= 30000 && time > 12000) this->starRecords[idx] = 2;
    else if (time <= 12000) this->starRecords[idx] = 3;
    else ;

    if (this->stars[idx]->layout() != NULL) {
        delete this->stars[idx];
        this->stars[idx] = new QWidget(this->scrollArea);
        this->stars[idx]->setFixedSize(130, 50);
        this->stars[idx]->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }

    QHBoxLayout *layout = new QHBoxLayout(this->stars[idx]);
    layout->setSpacing(5);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    for (int j = 0; j < 3; j++) {
        QLabel *singleStar = new QLabel(this->scrollArea);
        if (j >= starRecords[idx]) {
            singleStar->setPixmap(QPixmap("://resources/images/star_dark.png").scaled(35, 35));
        } else {
            singleStar->setPixmap(QPixmap("://resources/images/star.png").scaled(35, 35));
        }
        layout->addWidget(singleStar);
    }
    this->stars[idx]->setLayout(layout);
    determineGeometry();
}
