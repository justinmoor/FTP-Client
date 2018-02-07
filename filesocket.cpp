#include "filesocket.h"

FileSocket::FileSocket(QObject *parent) : QTcpSocket(parent)
{

}

void FileSocket::connectFileSocket(QString address, quint16 port){
    connectToHost(address, port);
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void FileSocket::sendFile(QString fileName){
    QFile file(fileName);
    QByteArray buffer;

    if(file.open(QIODevice::ReadOnly)){
        qDebug() << "File openend";
    }

    buffer = file.readAll();

    write(buffer);

    file.close();
}

void FileSocket::receiveFile(QString fileName){
    this->fileName = fileName;
}

void FileSocket::readyRead(){
    QFile file(fileName);
    QByteArray buffer = readAll();

    file.write(buffer);

    file.close();
}
