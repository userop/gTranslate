#include "include/gtranslate.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QException>
#include <QSslConfiguration>


GTranslate::GTranslate(QObject *parent)
    : QObject{parent},
      netManager(this),
      netRequest(),
      netReply(nullptr),
      host("https://translate.google.cn"),
      uriPath("/_/TranslateWebserverUi/data/batchexecute")
{
    QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    netRequest.setSslConfiguration(conf);
    netRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");
    netRequest.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36");

}

void GTranslate::setUrlDomain(QString domain)
{
    if (host.endsWith(domain))
        return;
    host = QString("http://translate.google.%1").arg(domain);
}

void GTranslate::transText(QString text, QString src, QString tgt)
{
    // handle text to translating
    QJsonArray textArr, num={1}, paramArr, rpc1, rpc2, rpc3;
    QByteArray textByt, data;
    textArr = {text, src, tgt, true};
    paramArr = {textArr, num};
    QJsonDocument textDoc(paramArr);
    textByt = textDoc.toJson(QJsonDocument::Compact);
    rpc1 = {"MkEWBc", QString(textByt), QJsonValue::Null, "generic"};
    rpc2.append(rpc1);
    rpc3.append(rpc2);
    QJsonDocument rpcDoc(rpc3);
    data = "f.req=" + rpcDoc.toJson(QJsonDocument::Compact).toPercentEncoding() + "&";
    // post request
    QUrl url(host + uriPath);
    netRequest.setUrl(url);
    netRequest.setRawHeader("Referer", host.toLocal8Bit());
    netReply = netManager.post(netRequest, data);

    connect(netReply, &QNetworkReply::readyRead, this, &GTranslate::slotRead);
    connect(netReply, &QNetworkReply::errorOccurred, this, &GTranslate::slotNetErr);
    connect(netReply, &QNetworkReply::sslErrors, this, &GTranslate::slotSslErr);
}

void GTranslate::slotRead()
{
    QByteArray line, content = netReply->readAll();
    QList<QByteArray> lines = content.split('\n');
    for (int i(0); i < lines.length();)
    {
        line = lines[i++];
        if (line.contains("MkEWBc"))
        {
            QJsonParseError jsErr;
            QJsonDocument resDoc, textDoc;
            QJsonArray resArr, textArr, sentences;
            resDoc = QJsonDocument::fromJson(line, &jsErr);
            if (QJsonParseError::NoError == jsErr.error)
            {
                textDoc = QJsonDocument::fromJson(resDoc[0][2].toString().toLocal8Bit(), &jsErr);
                if (QJsonParseError::NoError == jsErr.error)
                {
                    resArr = textDoc[1][0].toArray();
                    if (resArr.count() == 1)
                    {
                        textArr = textDoc[1][0][0].toArray();
                        if (textArr.count() <= 5)
                        {
                            emit translated(textArr[0].toString());
                        }else
                        {
                            QString transText;
                            sentences = textArr[5].toArray();
                            for (int j(0); j < sentences.count(); j++)
                            {
                                transText += textDoc[1][0][0][5][j][0].toString() + " ";
                            }
                            emit translated(transText);
                        }
                    } // else if count is 2, it's a other result.
                }
            }
        }
    }
}

void GTranslate::slotNetErr()
{

}

void GTranslate::slotSslErr()
{

}

GTranslate::~GTranslate()
{

}
