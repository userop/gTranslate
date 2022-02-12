#ifndef TEXTFROMCLIP_H
#define TEXTFROMCLIP_H

#include "include/gtranslate.h"
#include <QObject>
#include <QClipboard>
#include <QGuiApplication>
#include <QClipboard>

class TextFromClip : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString transText READ transText NOTIFY transTextChanged)
    Q_PROPERTY(QPoint cursor READ cursor NOTIFY cursorChanged)
public:
    explicit TextFromClip(QObject *parent = nullptr);
    ~TextFromClip();
    /* */
    Q_INVOKABLE QString transText();
    Q_INVOKABLE QPoint cursor();

public slots:
    void boardChanged(QClipboard::Mode);
    void receiveTrans(QString text);

signals:
    void transTextChanged();
    void cursorChanged();

private:
    QPoint _cursor;
    QString _transText, selectText;
    QClipboard *board;
    GTranslate _trans;
};

#endif // TEXTFROMCLIP_H
