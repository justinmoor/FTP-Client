#ifndef FTP_H
#define FTP_H

#include <QTcpSocket>
#include <QVector>
#include "fileinfo.h"

class Ftp : public QObject
{
    Q_OBJECT
public:
    Ftp(QObject *parent = nullptr);
    ~Ftp();

    //FTP Commands
    bool connectToHost(QString host, quint16 port); // Done
    void disconnectFromHost(); // Done
    void login(QString &username, QString &password);
    QVector<FileInfo> list(QString path = ""); // Done
    void cd(QString &path);
    void get(QString &filename);
    void put(QString &filename);
    void mkdir(QString &dirname);
    void rmdir(QString &dirname);

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void ready();

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketConnectionClosed();
    void socketBytesWritten(qint64);

private:
    QTcpSocket *socket;
    QByteArray bytesFromSocket;
    void parseDir(const QByteArray &buffer, FileInfo *info);
};

#endif // FTP_H
