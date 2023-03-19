#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "structs.h"

#include <QObject>

#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <qbluetoothdevicediscoveryagent.h>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QBluetoothAddress>


class BlueTooth : public QObject
{
    Q_OBJECT
public:
    explicit BlueTooth(QObject *parent = nullptr);

public slots:
    void startSearch();
    void sendStructure(QVector<Block> *structure);
    void sendAudioSucess();
    void sendCompletion();

private slots:
    void BTDiscover(QBluetoothDeviceInfo info);
    void BTConnect();
    void BTReSearching();
    void BTReadData();
    void BTSendData(Package package);
    void BTConnected();
    void BTUnableConnect(QBluetoothSocket::SocketError err);

signals:
    void EndSearch();
    void Searching();
    void Connecting();
    void Connected();
    void ConncetFaild();
    void ConncetLost();

    void ComfirmTrue();
    void ComfirmFalse();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QBluetoothLocalDevice *localDevice = nullptr;
    QString BTaddress;
    QBluetoothSocket *socket = nullptr;

};

#endif // BLUETOOTH_H
