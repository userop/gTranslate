#ifndef TEXTFROMCLIP_H
#define TEXTFROMCLIP_H

#include "include/gtranslate.h"

#include <QHotkey>
#include <QObject>
#include <QClipboard>
#include <QGuiApplication>

class TextFromClip : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString transText READ transText NOTIFY transTextChanged)
    Q_PROPERTY(QPoint cursor READ cursor NOTIFY cursorChanged)
    Q_PROPERTY(bool hide READ hide NOTIFY hideChanged)
public:
    explicit TextFromClip(QObject *parent = nullptr);
    ~TextFromClip();
    /* */
    Q_INVOKABLE QString transText();
    Q_INVOKABLE QPoint cursor();
    Q_INVOKABLE bool hide();

public slots:
    void boardChanged(QClipboard::Mode);
    void receiveTrans(QString text);
    void callHotKey();

signals:
    void transTextChanged();
    void cursorChanged();
    void hideChanged();

private:
    bool _hide;
    QPoint _cursor;
    QString _transText, selectText;
    QClipboard *board;
    GTranslate _trans;
    QHotkey *hotkey;
};

#endif // TEXTFROMCLIP_H
