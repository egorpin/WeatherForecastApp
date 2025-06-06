#pragma once

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class FileDownloader : public QObject {
    Q_OBJECT
 public:
    explicit FileDownloader(QUrl imageUrl, QObject* parent = 0);
    virtual ~FileDownloader();
    QByteArray downloadedData() const;

 signals:
    void downloaded(QByteArray);

 private slots:
    void fileDownloaded(QNetworkReply* pReply);

 private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
};
