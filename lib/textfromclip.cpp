#include "include/textfromclip.h"

#include <QCursor>

TextFromClip::TextFromClip(QObject *parent):
    QObject(parent),
    _hide(false),
    _trans(parent),
    hotkey(new QHotkey(QKeySequence("Ctrl+Shift+Q"), true, parent))
{
    board = QGuiApplication::clipboard();
    _trans.setUrlDomain("cn");

    connect(board, &QClipboard::changed, this, &TextFromClip::boardChanged);
    connect(&_trans, &GTranslate::translated, this, &TextFromClip::receiveTrans);
    connect(hotkey, &QHotkey::activated, this, &TextFromClip::callHotKey);
}


QString TextFromClip::transText()
{
    return _transText;
}

QPoint TextFromClip::cursor()
{
    return _cursor;
}

bool TextFromClip::hide()
{
    return _hide;
}

void TextFromClip::receiveTrans(QString text)
{
#ifdef DEBUG_MODE
    qDebug() << "received text: " << text;
#endif
    _transText = text;
    emit transTextChanged();
    emit cursorChanged();
}

void TextFromClip::boardChanged(QClipboard::Mode mode)
{
    if (_hide){
        return;
    }
    if (mode == QClipboard::Selection)
    {        
        QString content(board->text(QClipboard::Selection));
#ifdef DEBUG_MODE
        qDebug() << content;
#endif
        if (!content.isEmpty())
        {
            selectText = content;
            _cursor = QCursor::pos();
#ifdef DEBUG_MODE
            qDebug() << "translate content";
#endif
            _trans.transText(content);
        }
    }
}

void TextFromClip::callHotKey()
{
    _hide = !_hide;
    emit hideChanged();
}

TextFromClip::~TextFromClip()
{

}
