#include "../include/document.h"

#include <QtGui>

Document::Document(QObject *parent)
    : QObject{parent}
{

}

QString Document::file()
{
    return this->_fileName;
}

QString Document::text()
{
    return this->_content;
}

void Document::setFile(QString file)
{
// checkout text file, if unknown or unable decoded
    qDebug() << "File: " << file;
}

void Document::setText(QString text)
{
    _content = text;
    emit textChanged();
}

bool Document::isBinary(const QByteArray &content)
{

}

Document::~Document()
{

}
