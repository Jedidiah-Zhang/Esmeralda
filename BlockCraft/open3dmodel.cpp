#include "open3dmodel.h"
#include <QProcess>
#include <QStringList>
#include <QString>
#include <QDebug>

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
        arguments << "C:/Users/anuhg/Documents/Southampton/_Lessons/Year2/Coursework/D4/QTAPPDESIGN/QTAPPGUI/GUI/PythonScripts/QTProcessingRequests.py" << bits;


        QProcess *Request = new QProcess();
        Request->startDetached(program, arguments);
        Request->waitForFinished();

        delete Request;

        emit FinishedSending();
    }

}

void Open3DModel::run(){


    this-> process->execute("C:/Users/anuhg/Documents/Southampton/_Lessons/Year2/Coursework/D4/GUIREPOv1/Esmeralda/Application/3DModel/bats/Run3DRenderer.bat");

}

void Open3DModel::kill(){

   this-> process->kill();
}

