#include "ftp.h"

Ftp::Ftp(QObject *parent) : QObject(parent){

}

Ftp::~Ftp(){
  //  delete socket;
}


void Ftp::connectToHost(QString host, quint16 port, QString username, QString password){

    this->username = username;
    this->password = password;
    this->host = host;

    socket = new QTcpSocket(this);

    bytesFromSocket.clear();

    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(disconnected()),socket,SLOT( deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketConnectionClosed()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));

    socket->connectToHost(host, port);
}

void Ftp::error(){
    emit message("Can't connect to host!");
}

void Ftp::socketBytesWritten(qint64 bytes){
    qDebug() << "written";
}

void Ftp::socketReadyRead(){
    qDebug() << "Ready read";
    QString response = receiveResponse();
    if(response.startsWith("230")){
        connectFileSocket(host, response.split(" ").at(3).toInt());
        emit connectedToServer();
    } else if(response.startsWith("250")){

    }
}

void Ftp::socketConnectionClosed(){
    emit disconnectedFromServer();
}

// Authenicate
void Ftp::socketConnected(){
    sendCommand(("LOGIN " + username + " " + password).toLocal8Bit());
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

//void Ftp::parseList(QString list){
//    QStringList lines = list.split("\n");
//    QVector<FileInfo> files;
//    foreach(QString line, lines){
//        FileInfo file;
//        parseDir(line, &file);
//        files.push_back(file);

//    }

//    emit listDone(lines);
//}

QVector<FileInfo> Ftp::list(QString path){
    QVector<FileInfo> files;
    sendCommand("LIST " + path.toLocal8Bit());
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
    socket->flush();
    socket->write(command+"\r\n");
}

QString Ftp::receiveResponse(){
    QString res = socket->readAll();
    emit response(res.trimmed());
    return res.trimmed();
}

void Ftp::put(QString &filePath, QString &fileName){
    socket->write("STOR " + fileName.toLocal8Bit());
    fileSocket->sendFile(filePath);
}

void Ftp::connectFileSocket(QString address, quint16 port){
    fileSocket = new FileSocket();

    fileSocket->connectFileSocket(address, port);
}
