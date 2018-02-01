#ifndef FTP_H
#define FTP_H

#include <QTcpSocket>
#include <QVector>
#include <QWaitCondition>
#include <QMutex>
#include <QDataStream>
#include "fileinfo.h"

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
    QVector<FileInfo> list(QString path = ""); // Done
    void cd(QString &path);
    void get(QString &filename);
    void put(QString &filename);
    void mkdir(QString &dirname);
    void rmdir(QString &dirname);

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void response(QByteArray);
    void message(QString);

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketConnectionClosed();
    void socketBytesWritten(qint64);
    void error();

private:
    QTcpSocket *socket;
    QByteArray bytesFromSocket;
    void parseDir(const QByteArray &buffer, FileInfo *info);
    void sendCommand(QByteArray command);
    QString receiveResponse();

    QString username;
    QString password;


};

#endif // FTP_H
