#include "Filedownloader.hpp"

FileDownloader::FileDownloader(QUrl imageUrl, QObject* parent) : QObject(parent) {
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() {
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();

    pReply->deleteLater();
    emit downloaded(m_DownloadedData);
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
