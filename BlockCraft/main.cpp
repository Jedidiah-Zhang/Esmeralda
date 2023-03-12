#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    // load font file
//    int fontId = QFontDatabase::addApplicationFont(":/font/resources/fonts/dearJoe5CASUAL-P.otf");
//    qDebug() << fontId;
//    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
//    QFont font;
//    if (fontFamilies.size() > 0) {
//        font.setFamily(fontFamilies.at(0));
//        font.setPixelSize(14);
//    }
//    QApplication::setFont(font);

    // start application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
