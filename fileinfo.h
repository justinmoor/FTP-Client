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

    qint16 getSize();
    void setSize(qint16 size);

private:
    QString name;
    QDateTime lastModified;
    bool dir;
    bool file;
    qint16 size;
};

#endif // FILEINFO_H
