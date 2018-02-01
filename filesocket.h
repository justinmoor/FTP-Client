#ifndef FILESOCKET_H
#define FILESOCKET_H

#include <QTcpSocket>

class FileSocket : public QTcpSocket
{
    Q_OBJECT
public:
    FileSocket(QObject *parent);
    void connectFileSocket(QString address, quint16 port);
    void sendFile(QByteArray data);
    void receiveFile(QByteArray data);

private:
    bool uploading = false;
};

#endif // FILESOCKET_H
