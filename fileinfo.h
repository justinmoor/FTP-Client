#ifndef FILEINFO_H
#define FILEINFO_H

//#include <QString>
#include <QDateTime>


class FileInfo {

public:
    FileInfo();
    QString getName();
    void setName(const QString name);

    QDateTime getLastModified();
    void setLastModified(const QDateTime dt);

    bool isDir();
    void setDir(bool dir);

    bool isFile();
    void setFile(bool file);

    long getSize();
    void setSize(long size);

private:
    QString name;
    QDateTime lastModified;
    bool dir;
    bool file;
    long size;
};

#endif // FILEINFO_H
