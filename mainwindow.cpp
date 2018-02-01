#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    disconnect = false;
    ui->setupUi(this);
    ui->localTab->removeTab(1);
    ui->remoteTab->removeTab(1);
    ui->logTab->removeTab(1);
    ui->downloadProgress->setValue(0);
    ui->remoteFiles->setEnabled(false);
 //   ui->downloadButton->setEnabled(false);

    fileModel = new QFileSystemModel;
    fileModel->setRootPath(QDir::currentPath());
    ui->localFiles->setModel(fileModel);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete ftp;
    delete fileModel;
}

void MainWindow::on_connectButton_clicked()
{
    if(!disconnect){

        ftp = new Ftp();

        connect(ftp, SIGNAL(connectedToServer()), this, SLOT(connected()));
        connect(ftp, SIGNAL(disconnectedFromServer()), this, SLOT(disconnected()));
        connect(ftp, SIGNAL(response(QByteArray)), this, SLOT(response(QByteArray)));
        connect(ftp, SIGNAL(message(QString)), this, SLOT(message(QString)));

        QString address = ui->address->text();
        quint16 port = ui->port->text().toInt();

        QString username = ui->username->text();
        QString password = ui->password->text();

        ftp->connectToHost(address, port, username, password);
    } else {
        ftp->disconnectFromHost();
        ui->connectButton->setText("Connect!");
        disconnect = false;
    }
}

void MainWindow::on_downloadButton_clicked(){

   // socket->write("C:");
}

void MainWindow::getRemoteFiles(QString path){
    QVector<FileInfo> files = ftp->list();
    qDebug() << files.size();
    foreach(FileInfo file, files){

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, file.getName());
        item->setText(1, (file.isDir()) ? "" : QString::number(file.getSize()));
        item->setText(2, (file.isDir()) ? "File Folder" : "File");
        item->setText(3, file.getLastModified().toString("MM-dd-yy  hh:mm"));

        QPixmap pixmap(file.isDir() ? ":/icons/icons/folder.png" : ":/icons/icons/file.png");
        item->setIcon(0, pixmap);
        ui->remoteFiles->addTopLevelItem(item);
    }
}

void MainWindow::connected(){
    ui->connectButton->setText("Disconnect!");
    disconnect = true;
  //  getRemoteFiles();
    ui->remoteFiles->setEnabled(true);
}


void MainWindow::disconnected(){
    ui->remoteFiles->clear();
    ui->remoteFiles->setEnabled(false);
    ui->logTextEdit->appendPlainText("You have been disconnected!");
    ui->connectButton->setText("Connect!");
    disconnect = false;
}

void MainWindow::response(QByteArray response){
    ui->logTextEdit->appendPlainText(response);
}

void MainWindow::message(QString message){
    ui->logTextEdit->appendPlainText(message);
}


