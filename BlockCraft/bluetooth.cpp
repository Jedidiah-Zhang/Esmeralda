#include "bluetooth.h"

#include <QTimer>

BlueTooth::BlueTooth(QObject *parent)
    : QObject{parent}
{
    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    this->discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(BTDiscover(QBluetoothDeviceInfo)));
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BlueTooth::EndSearch);
}

void BlueTooth::startSearch()
{
    this->discoveryAgent->start();
    emit Searching();
}

void BlueTooth::sendStructure(QVector<Block> *structure)
{
    Package pkg;
    pkg.cmd = STRUCT;
    for (int i = 0; i < structure->size(); i++) {
        Block block = structure->at(i);
        uint16_t blockinfo = (block.tile << 12) | (block.height << 8) | (block.ID << 2) | block.rot;
        QByteArray data(reinterpret_cast<const char*>(&blockinfo), sizeof(uint16_t));
        pkg.data.append(data);
    }
    BTSendData(pkg);
}

void BlueTooth::sendAudioSucess()
{
    Package pkg;
    pkg.cmd = AUDIO;
    pkg.data = 0x00;
    BTSendData(pkg);
}

void BlueTooth::sendCompletion()
{
    Package pkg;
    pkg.cmd = COMPLET;
    BTSendData(pkg);
}

void BlueTooth::BTDiscover(QBluetoothDeviceInfo info)
{
    QString label = QString("%1 %2").arg(info.address().toString(), info.name());
    if (info.name() == BTNAME) {
        this->BTaddress = info.address().toString();
        this->BTConnect();
        this->discoveryAgent->stop();
    }
    qDebug() << label;
}

void BlueTooth::BTConnect()
{
//    qDebug() << "Scan Finished";
    emit Connecting();
    static QString serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
    this->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    this->socket->connectToService(QBluetoothAddress(this->BTaddress), QBluetoothUuid(serviceUuid),QIODevice::ReadWrite);
    connect(this->socket, &QBluetoothSocket::readyRead, this, &BlueTooth::BTReadData);
    connect(this->socket, &QBluetoothSocket::connected, this, &BlueTooth::BTConnected);
    connect(this->socket, &QBluetoothSocket::disconnected, this, &BlueTooth::BTReSearching);
    connect(this->socket, SIGNAL(errorOccurred(QBluetoothSocket::SocketError)),
            this, SLOT(BTUnableConnect(QBluetoothSocket::SocketError)));
}

void BlueTooth::BTReSearching()
{
    emit ConncetLost();

    QTimer::singleShot(3000, this, [=](){
        emit Searching();
        this->discoveryAgent->start();
    });
}

void BlueTooth::BTReadData()
{
    int cmd = socket->read(1).toHex().toInt(NULL, 16);
    switch (cmd) {
    case COMFIRMT:
        emit ComfirmTrue();
        break;
    case COMFIRMF:
        emit ComfirmFalse();
        break;
    default:
        qDebug() << cmd << socket->readLine();
        break;
    }
}

void BlueTooth::BTSendData(Package package)
{
    uint8_t cmd, data;
    switch (package.cmd) {
    case STRUCT:
        socket->write(QByteArray::number(0b00010000)); // command
        socket->write(QByteArray::number(package.data.length()/2)); // length
        socket->write(package.data); // blocks
        break;
    case LED:
//        socket->write("0011");
//        socket->write(package.data);
        break;
    case AUDIO:
        cmd = 0b0011;
        data = (uint8_t) package.data.toInt();
        socket->write(QByteArray::number((cmd << 4) | data));
        break;
    case COMPLET:
        socket->write(QByteArray::number(0b01000000));
        break;
    default:
        break;
    }

//    qDebug() << "Send Data";
}

void BlueTooth::BTConnected()
{
    emit Connected();
    qDebug() << "Bluetooth Connected.";
}

void BlueTooth::BTUnableConnect(QBluetoothSocket::SocketError err)
{
    if (err == QBluetoothSocket::SocketError::ServiceNotFoundError) {
        emit ConncetFaild();

        QTimer::singleShot(3000, this, [=](){
            emit Searching();
            this->discoveryAgent->start();
        });
    }
}
