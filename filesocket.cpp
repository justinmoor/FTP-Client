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
    fileNameSet = true;
}

void FileSocket::readyRead(){
    if(!command.startsWith("LIST")){
        if(fileNameSet){
            QFile file(fileName);

            if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
                qDebug() << file.errorString();
                return;
            }

            emit bytesReceived(bytesAvailable());

            QByteArray buffer = readAll();

            file.write(buffer);
            file.close();

            if(!isReadable()){
                fileNameSet = false;
            }

        }
    } else {
        QString fileList = readAll();
        emit listReceived(fileList);
    }
}

void FileSocket::fileSocketConnected(){
    qDebug () << "File socket connected";
}

void FileSocket::setReceivingList(bool recList){
    this->receivingList = recList;
}

void FileSocket::currentCommand(QString command){
   this->command = command;
}
