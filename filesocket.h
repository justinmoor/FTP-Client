#ifndef FILESOCKET_H
#define FILESOCKET_H

#include <QTcpSocket>
#include <QFile>

class FileSocket : public QTcpSocket
{
    Q_OBJECT
public:
    FileSocket(QObject *parent = 0);
    void connectFileSocket(QString address, quint16 port);
    void sendFile(QString fileName);
    void receiveFile(QString fileName);
private:
    bool uploading = false;
    QString fileName;

public slots:
    void readyRead();
};

#endif // FILESOCKET_H
