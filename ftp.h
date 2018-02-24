#ifndef FTP_H
#define FTP_H

#include <QTcpSocket>
#include <QVector>
#include <QWaitCondition>
#include <QMutex>
#include <QDataStream>
#include "fileinfo.h"
#include "filesocket.h"

class Ftp : public QObject
{
    Q_OBJECT
public:
    Ftp(QObject *parent = 0);
    ~Ftp();

    //FTP Commands
    void connectToHost(QString host, quint16 port, QString username, QString password); // Done
    void disconnectFromHost(); // Done
    void login(QString &username, QString &password); // Done
    void list(QString path = "/"); // Done
    void cd(QString &path); // Done
    void get(QString &fileName, QString &savePath);
    void put(QString &filePath, QString &fileName); // Done;
    void mkdir(QString &dirname); // Done
    void rmdir(QString &dirname);

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void response(QString);
    void message(QString);
    void listDone(QVector<FileInfo>);
    void currentCommand(QString command);
    void toDownloadBar(qint64);
    void toUploadBar(qint64);

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketConnectionClosed();
    void socketBytesWritten(qint64);
    void socketBytesReceived(qint64);
    void error();
    void parseList(QString list);

private:
    QTcpSocket *socket;
    FileSocket *fileSocket;
    QByteArray bytesFromSocket;
    void parseDir(const QByteArray &buffer, FileInfo *info);
    void sendCommand(QByteArray command);
    QString receiveResponse();
    void connectFileSocket(QString address, quint16 port);
    QString username;
    QString password;
    QString host;
};

#endif // FTP_H
