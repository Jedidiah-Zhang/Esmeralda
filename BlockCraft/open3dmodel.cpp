#include "open3dmodel.h"

#include <QProcess>
#include <QStringList>
#include <QString>
#include <QResource>
#include <QCoreApplication>
#include <QFile>

Open3DModel::Open3DModel()
{
    this->process = new QProcess();
}

void Open3DModel::send(QString bits)
{
    //Sends data to rendered via wifi
    if(bits.size() == 27){

        QString program = "python";

        QStringList arguments;
        arguments << ":/codes/PythonScripts/BluetoothModule.py" << bits;

        QProcess *Request = new QProcess();
        Request->startDetached(program, arguments);
        Request->waitForFinished();

        delete Request;

        emit FinishedSending();
    }
}

void Open3DModel::run()
{
    qDebug() << "Run bat";
//    qDebug() << this->process->execute(":/codes/bats/Run3DRenderer.bat");

    QResource batResource(":/codes/bats/Run3DRenderer.bat");
    if (!batResource.isValid()) {
        qDebug() << "Failed to load bat file.";
    }

    QString batFileName = QCoreApplication::applicationDirPath() + "/Run3DRenderer.bat";
    QFile batFile(batFileName);
    if (!batFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to create temp file.";
    }
    batFile.write(reinterpret_cast<const char *>(batResource.data()), batResource.size());
    batFile.close();

    process->start("cmd", QStringList() << "/c" << batFileName);
    if (!process->waitForFinished()) {
        qDebug() << "Failed to execute bat file.";
    }
    qDebug() << "Exit code:" << process->exitCode();

    QFile::remove(batFileName);
}

void Open3DModel::kill()
{
    qDebug() << "Kill process";
//    this->process->kill();
}

