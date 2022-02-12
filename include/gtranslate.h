#ifndef GTRANSLATE_H
#define GTRANSLATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


/*
 * Thank to google translate
 * Sun 19 Dec
 * google-trans-new 1.1.9
 */
class GTranslate : public QObject
{
    Q_OBJECT
public:
    explicit GTranslate(QObject *parent = nullptr);
    ~GTranslate();
    void setUrlDomain(QString);
//    bool set
    void transText(QString text, QString src="auto", QString tgt="zh-cn");

public slots:
    void slotRead();
    void slotNetErr();
    void slotSslErr();

public:
    QNetworkAccessManager netManager;
    QNetworkRequest netRequest;
    QNetworkReply *netReply;

private:
    QString host, uriPath;

signals:
    void translated(QString text);

};

#endif // GTRANSLATE_H
