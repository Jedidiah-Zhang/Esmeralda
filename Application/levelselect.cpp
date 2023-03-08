#include "levelselect.h"

#include <QTimer>
#include <QImage>

levelSelect::levelSelect(QWidget *parent)
    : QWidget(parent)
{
//    this->setFixedSize(500, 800);
//    this->setWindowTitle("Select Level");

    this->parent = parent;
    this->W_WIDTH = parent->size().width();
    // determine how many columns able to display in terms of the width of the window.
    this->columns = (W_WIDTH-20)/220;

    this->scrollArea = new QScrollArea(this);
    this->selectGrid = new QFrame(this);
    this->gridLayout = new QGridLayout(this);

    // back button at top left
    PushButton *backBtn = new PushButton(QSize(100, 30), ":/resources/images/back.png");
    backBtn->setParent(this);
    backBtn->move(5, 5);

    // create buttons
    for (int i = 0; i < 15; i++) {
        PushButton *button = new PushButton(QSize(200, 200), "://resources/images/defaultLevelBackground.png");
        this->buttons.push_back(button);
        gridLayout->addWidget(button, i/columns, i%columns, 1, 1);
        button->setStyleSheet("QPushButton{background-color:gray;"
                              "color:white;"
                              "border-radius:10px;"
                              "border:2px groove gray;"
                              "border-style:outset;}");

        connect(button, &PushButton::clicked, this, [=](){
            button->bounce(true);
            button->bounce(false);

            qDebug() << "i = " << i;
        });
    }

    // send signal as back button been clicked
    connect(backBtn, &PushButton::clicked, this, [=](){
        backBtn->bounce(true);
        backBtn->bounce(false);

        QTimer::singleShot(100, this, [=](){
            emit this->backButtonClicked();
        });
    });

    determineGeometry();
}

void levelSelect::determineGeometry()
{
    this->W_WIDTH = this->parent->size().width();
    this->W_HEIGHT = this->parent->size().height();
    this->columns = (W_WIDTH-20)/220;

    // rearrange the buttons
    for (int i = 0; i < this->buttons.length(); i++) {
        gridLayout->addWidget(buttons[i], i/columns, i%columns, 1, 1);
    }

    // change the spacing as the window size changed
    gridLayout->setAlignment(Qt::AlignTop);
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
    scrollArea->setAutoFillBackground(true);
    // set background in the selection area
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(QImage("://resources/images/Bricks.png").scaled(64, 64)));
    scrollArea->setPalette(pal);
}
