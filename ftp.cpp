#include "ftp.h"

Ftp::Ftp(QObject *parent) : QThread(parent){

}

Ftp::~Ftp(){
  //  delete socket;
}


bool Ftp::connectToHost(QString host, quint16 port, QString username, QString password){


    QMutexLocker locker(&mutex);
    this->hostName = host;
    this->port = port;
    this->username = username;
    this->password = password;

    if(!isRunning()){
        start();
    } else {
        cond.wakeOne();
    }

    bytesFromSocket.clear();


//    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
//    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
//    connect(socket, SIGNAL(disconnected()), this, SLOT(socketConnectionClosed()));
//    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));

  //  connect(socket,SIGNAL(disconnected()),socket,SLOT( deleteLater()));


}

void Ftp::run(){
    QTcpSocket socket;

    qDebug() << "Connect";
    socket.connectToHost(hostName, port);
    qDebug() << "Connecting";
    if(!socket.waitForConnected()){
        return;
    }
    qDebug() << "Connected";
}

void Ftp::socketBytesWritten(qint64 bytes){

}

void Ftp::socketReadyRead(){
    qDebug() << "Ready read";
    if(receiveResponse().startsWith("430")) socket->disconnectFromHost();
}

void Ftp::socketConnectionClosed(){
    emit disconnectedFromServer();
}

// Authenicate
void Ftp::socketConnected(){
    sendCommand(("LOGIN " + username + " " + password).toLocal8Bit());
    emit connectedToServer();
}

void Ftp::disconnectFromHost(){
    socket->disconnectFromHost();
}

void Ftp::parseDir(const QByteArray &buffer, FileInfo *info){
    QString bufferStr = QString::fromLatin1(buffer).trimmed();

    QRegExp dosPattern(QLatin1String("^(\\d\\d-\\d\\d-\\d\\d\\d?\\d?\\ \\ \\d\\d:\\d\\d[AP]M)\\s+"
                                         "(<DIR>|\\d+)\\s+(\\S.*)$"));

    if (dosPattern.indexIn(bufferStr) == 0) {
        QStringList tokens = dosPattern.capturedTexts();
        //    parseDosDir(dosPattern.capturedTexts(), userName, info);
        QString name = tokens.at(3);
        info->setName(name);
     //   info->setSymLink(name.toLower().endsWith(QLatin1String(".lnk")));

        if (tokens.at(2) == QLatin1String("<DIR>")) {
            info->setFile(false);
            info->setDir(true);
        } else {
            info->setFile(true);
            info->setDir(false);
            info->setSize(tokens.at(2).toLongLong());
        }

  //      info->setReadable(true);
  //      info->setWritable(info->isFile());

        QDateTime dateTime;

    #ifndef QT_NO_DATESTRING
        dateTime = QLocale::c().toDateTime(tokens.at(1), QLatin1String("MM-dd-yy  hh:mmAP"));
        if (dateTime.date().year() < 1971) {
            dateTime.setDate(QDate(dateTime.date().year() + 100,
                                   dateTime.date().month(),
                                   dateTime.date().day()));
        }
    #endif

          info->setLastModified(dateTime);
       }
}

QVector<FileInfo> Ftp::list(QString path){
    QVector<FileInfo> files;

    sendCommand("LIST");

    socket->waitForReadyRead();

    while(socket->canReadLine()){
        FileInfo file;
        bytesFromSocket = socket->readLine();
        parseDir(bytesFromSocket, &file);

        files.push_back(file);

        bytesFromSocket.clear();
    }
    return files;
}

void Ftp::sendCommand(QByteArray command){
    socket->write(command);
}

QString Ftp::receiveResponse(){
    QString res = socket->readAll();
    emit response(res.toLocal8Bit());
    socket->flush();
    return res;
}
