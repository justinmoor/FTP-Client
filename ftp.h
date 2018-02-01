#ifndef FTP_H
#define FTP_H

#include <QTcpSocket>
#include <QVector>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include "fileinfo.h"

class Ftp : public QThread
{
    Q_OBJECT
public:
    Ftp(QObject *parent = 0);
    ~Ftp();

    //FTP Commands

    bool connectToHost(QString host, quint16 port, QString username, QString password); // Done
    void run() override;
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
    void response(QByteArray);

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketConnectionClosed();
    void socketBytesWritten(qint64);

private:
    QTcpSocket *socket;
    QByteArray bytesFromSocket;
    void parseDir(const QByteArray &buffer, FileInfo *info);
    void sendCommand(QByteArray command);
    QString receiveResponse();

    QString username;
    QString password;

    QString hostName;
    quint16 port;
    QMutex mutex;
    QWaitCondition cond;
};

#endif // FTP_H
