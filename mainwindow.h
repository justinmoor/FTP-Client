#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTcpSocket>
#include <QVector>
#include <QTreeWidget>
#include <QInputDialog>

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
    void response(QString);
    void message(QString);

    void on_remoteFiles_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_uploadButton_clicked();

    void on_localFiles_clicked(const QModelIndex &index);
    void populateList(QVector<FileInfo>);
    void menu(const QPoint &click);

    void on_remoteFiles_itemClicked(QTreeWidgetItem *item, int column);
    void populateDownloadBar(qint64 bytes);
    void populateUploadBar(qint64 bytes);

private:
    Ui::MainWindow *ui;
    Ftp *ftp;
    bool disconnect;
    QFileSystemModel *fileModel;
    void getRemoteFiles(QString path = "/");
    int bytesDownloaded = 0;
    int bytesUploaded = 0;
};

#endif // MAINWINDOW_H
