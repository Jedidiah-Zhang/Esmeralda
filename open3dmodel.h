#ifndef OPEN3DMODEL_H
#define OPEN3DMODEL_H

#include <QObject>
#include <QProcess>

class Open3DModel: public QObject
{
    Q_OBJECT

public:
    Open3DModel();
    void run();

public slots:
    void send(QString bits);
    void kill();

signals:
    void FinishedSending();

private:
    QProcess *process;

};



#endif // OPEN3DMODEL_H
