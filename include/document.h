#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>

/*
 * 若想数据元素中的方法可以被QML直接调用有2种方法：
 *   在函数申明前添加 Q_INVOKABLE 宏
 *   申明成public slots
 */
/*
 * Open file
 * load content and try to transform to richText
 * Most important is to divide text(article) to paragraph,sentence,
 */
class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString text WRITE setText NOTIFY textChanged)
public:
    explicit Document(QObject *parent = nullptr);
    ~Document();
    /* 使用Q_INVOKABLE来修饰成员函数，目的在于被修饰的成员函数能够被元对象系统所唤起。 */

    /*
     * 打开文件的方式规范三个要点：
     *    获取文件开头的16bit，判断文件类型是已知且支持的格式
     *    对于未知的文件，则判断文件不为二进制文件/算法要将中文也考虑在其中
     *    最后就是判断纯文本大小不能超过4M
     *
     *  XML 3C3F786D6C
     *  PDF 255044462D312E
     *  HTML 68746D6C3E
     *  DOC D0CF11E0
     */
    Q_INVOKABLE QString file();
    Q_INVOKABLE QString text();

public slots:
    void setFile(QString file);
    void setText(QString text);

private:
    bool isBinary(const QByteArray &);


signals:
    void fileChanged();
    void textChanged();

private:
    QString _fileName;
    QString _content;
};

#endif // DOCUMENT_H
