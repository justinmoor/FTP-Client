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
    QVector<FileInfo> list(QString path = "/"); // Done
    void cd(QString &path);
    void get(QString &filename);
    void put(QString &filePath, QString &fileName); // Upload;
    void mkdir(QString &dirname);
    void rmdir(QString &dirname);

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void response(QString);
    void message(QString);

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketConnectionClosed();
    void socketBytesWritten(qint64);
    void error();

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
