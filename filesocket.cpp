#include "filesocket.h"

FileSocket::FileSocket(QObject *parent) : QTcpSocket(parent)
{

}

void FileSocket::connectFileSocket(QString address, quint16 port){
    connectToHost(address, port);
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(connected()), this, SLOT(fileSocketConnected()));
}

void FileSocket::sendFile(QString fileName){
    QFile file(fileName);
    QByteArray buffer;

    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Something went wrong";
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

    if(!file.open(QFile::Append))
    {
        qDebug() << "Could not open file!";
        return;
    }

    file.write(buffer);
    file.close();
}

void FileSocket::fileSocketConnected(){
    qDebug () << "File socket connected";
}
