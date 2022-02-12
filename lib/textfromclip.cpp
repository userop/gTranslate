#include "include/textfromclip.h"

#include <QCursor>
#include <QDebug>

TextFromClip::TextFromClip(QObject *parent):
    QObject(parent),
    _trans(parent)
{
    board = QGuiApplication::clipboard();
    _trans.setUrlDomain("cn");

    connect(board, &QClipboard::changed, this, &TextFromClip::boardChanged);
    connect(&_trans, &GTranslate::translated, this, &TextFromClip::receiveTrans);
}


QString TextFromClip::transText()
{
    return _transText;
}

QPoint TextFromClip::cursor()
{
    return _cursor;
}

void TextFromClip::receiveTrans(QString text)
{
    _transText = text;
    emit transTextChanged();
    emit cursorChanged();
}

void TextFromClip::boardChanged(QClipboard::Mode mode)
{
    if (mode == QClipboard::Selection)
    {
        QString content(board->text(QClipboard::Selection));
        if (!content.isEmpty() && content != selectText)
        {
            selectText = content;
            _cursor = QCursor::pos();
            _trans.transText(content);
        }
    }
}

TextFromClip::~TextFromClip()
{

}
