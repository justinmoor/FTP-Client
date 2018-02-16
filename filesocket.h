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
    void setReceivingList(bool recList);
private:
    bool uploading = false;
    bool receivingList = false;
    QString fileName;
    QString command;

public slots:
    void readyRead();
    void fileSocketConnected();
    void currentCommand(QString command);

signals:
    void listReceived(QString list);
};

#endif // FILESOCKET_H
