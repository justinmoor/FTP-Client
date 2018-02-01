#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTcpSocket>
#include <QVector>
#include "ftp.h"
#include "fileinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connected();
    void disconnected();
    void on_connectButton_clicked();
    void on_downloadButton_clicked();
    void response(QByteArray);

private:
    Ui::MainWindow *ui;
    Ftp ftp;
    bool disconnect;
    QFileSystemModel *fileModel;
    void getRemoteFiles(QString path = "");
};

#endif // MAINWINDOW_H
