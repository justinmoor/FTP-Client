#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QDebug>
#include <QDirModel>


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
    ui->uploadProgress->setValue(0);
    ui->remoteFiles->setEnabled(false);
    ui->downloadButton->setEnabled(false);

    fileModel = new QFileSystemModel;
    fileModel->setRootPath(QDir::currentPath());
    ui->localFiles->setModel(fileModel);
    ui->uploadButton->setEnabled(false);
    ui->remoteFiles->setContextMenuPolicy(Qt::CustomContextMenu);
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
        ftp = new Ftp(); // Delete this

        connect(ftp, SIGNAL(connectedToServer()), this, SLOT(connected()));
        connect(ftp, SIGNAL(disconnectedFromServer()), this, SLOT(disconnected()));
        connect(ftp, SIGNAL(response(QString)), this, SLOT(response(QString)));
        connect(ftp, SIGNAL(message(QString)), this, SLOT(message(QString)));
        connect(ftp, SIGNAL(listDone(QVector<FileInfo>)), this, SLOT(populateList(QVector<FileInfo>)));
        connect(ftp, SIGNAL(toDownloadBar(qint64)), this, SLOT(populateDownloadBar(qint64)));
        connect(ftp, SIGNAL(toUploadBar(qint64)), this, SLOT(populateUploadBar(qint64)));

        QString address = ui->address->text();
        quint16 port = ui->port->text().toInt();

        QString username = (ui->username->text().isEmpty()) ? "anonymous" : ui->username->text();
        QString password = (ui->password->text().isNull()) ? "nopass" : ui->password->text();

        ftp->connectToHost(address, port, username, password);
    } else {
        ftp->disconnectFromHost();
        ui->connectButton->setText("Connect!");
        disconnect = false;

    }
}


void MainWindow::getRemoteFiles(QString path){
    ftp->list(path);
}

void MainWindow::connected(){
    ui->connectButton->setText("Disconnect!");
    disconnect = true;
    getRemoteFiles();
    ui->remoteFiles->setEnabled(true);
    connect(ui->remoteFiles, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menu(const QPoint &)));
}


void MainWindow::disconnected(){
    ui->remoteFiles->clear();
    ui->remoteFiles->setEnabled(false);
    ui->downloadButton->setEnabled(false);
    ui->uploadButton->setEnabled(false);
    ui->downloadProgress->setValue(0);
    ui->uploadProgress->setValue(0);
    ui->logTextEdit->appendPlainText("You have been disconnected!");
    ui->connectButton->setText("Connect!");
    disconnect = false;

}

void MainWindow::response(QString response){
    ui->logTextEdit->appendPlainText(response);
}

void MainWindow::message(QString message){
    ui->logTextEdit->appendPlainText(message);
}

void MainWindow::on_remoteFiles_itemDoubleClicked(QTreeWidgetItem *item, int column){
    QString path = item->text(0);
    getRemoteFiles(path);
}

void MainWindow::on_uploadButton_clicked(){
    bytesUploaded = 0;
    QModelIndex index = ui->localFiles->currentIndex();
    ui->uploadProgress->setMaximum(fileModel->size(index));
    QString filePath = fileModel->filePath(index);
    QString fileName = '"' + fileModel->fileName(index) + '"';
    ftp->put(filePath, fileName);
}

// TODO FIXEN!!!!!!!!
void MainWindow::on_downloadButton_clicked(){
    bytesDownloaded = 0;
    ui->downloadProgress->setMaximum(ui->remoteFiles->currentItem()->text(1).toInt());
    QString fileName = ui->remoteFiles->currentItem()->text(0);
    QModelIndex index = ui->localFiles->currentIndex();
    QString savePath = fileModel->filePath(index) + "/" + fileName;

    ftp->get(fileName, savePath);
}

void MainWindow::on_localFiles_clicked(const QModelIndex &index){
    ui->uploadButton->setEnabled((fileModel->fileInfo(index).isFile()) ? true : false);
}

void MainWindow::on_remoteFiles_itemClicked(QTreeWidgetItem *item, int column){
    ui->downloadButton->setEnabled((item->text(2) == "File") ? true : false);
}

void MainWindow::populateList(QVector<FileInfo> fileInfo){
    ui->remoteFiles->clear();
    QVector<FileInfo> files = fileInfo;
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

void MainWindow::menu(const QPoint &click){
    QMenu menu(this);
    QAction *u = menu.addAction(tr("Create directory"));
    QAction *a = menu.exec(ui->remoteFiles->viewport()->mapToGlobal(click));

    if(a == u){
        QString dirname = QInputDialog::getText(this, "Enter directory name", "Enter the name of the new directory");
        if(!dirname.isEmpty()){
           dirname  = '"' + dirname + '"';
           ftp->mkdir(dirname);
        }
    }
}

void MainWindow::populateDownloadBar(qint64 bytes){
    bytesDownloaded += bytes;
    ui->downloadProgress->setValue(bytesDownloaded);
}

void MainWindow::populateUploadBar(qint64 bytes){
    bytesUploaded += bytes;

    ui->uploadProgress->setValue(bytesUploaded);
}
