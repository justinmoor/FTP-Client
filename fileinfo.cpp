#include "fileinfo.h"

FileInfo::FileInfo()
{

}

QString FileInfo::getName(){
    return this->name;
}

void FileInfo::setName(const QString name){
    this->name = name;
}

QDateTime FileInfo::getLastModified(){
    return this->lastModified;
}

void FileInfo::setLastModified(const QDateTime dt){
    this->lastModified = dt;
}

bool FileInfo::isDir(){
    return this->dir;
}

void FileInfo::setDir(bool dir){
    this->dir = dir;
}

bool FileInfo::isFile(){
    return this->file;
}

void FileInfo::setFile(bool file){
    this->file = file;
}

long FileInfo::getSize(){
    return this->size;
}

void FileInfo::setSize(long size){
    this->size = size;
}




