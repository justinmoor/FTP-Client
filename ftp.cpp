#include "ftp.h"

Ftp::Ftp(QObject *parent) : QObject(parent){

}

Ftp::~Ftp(){
    delete socket;
}

bool Ftp::connectToHost(QString host, quint16 port){
    bytesFromSocket.clear();

    if(socket){
        delete socket;
        socket = 0;
    }

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketConnectionClosed()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));

    socket->connectToHost(host, port);

    if(!socket->waitForConnected(5000)){
        return false;
    } else {
        return true;
    }
}

void Ftp::socketBytesWritten(qint64 bytes){

}

void Ftp::socketReadyRead(){
    qDebug() << "Ready read";
    emit ready();
}

void Ftp::socketConnectionClosed(){
    emit disconnectedFromServer();
}

void Ftp::socketConnected(){
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

    socket->write("LIST");
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
