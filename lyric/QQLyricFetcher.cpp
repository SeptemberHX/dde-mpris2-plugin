//
// Created by ragdoll on 2021/5/18.
//

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include "QQLyricFetcher.h"

void QQLyricFetcher::requestForLyric(QString title, QString artist) {
    QNetworkRequest request;
    QUrl url(QString("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w=%1&format=json").arg(title+artist));
    request.setUrl(url);
    this->idManager->get(request);
}

QQLyricFetcher::QQLyricFetcher() {
    this->idManager = new QNetworkAccessManager(this);
    connect(this->idManager, &QNetworkAccessManager::finished, this, &QQLyricFetcher::songGetFinished);

//    this->lyricManager = new QNetworkAccessManager(this);
//    connect(this->lyricManager, &QNetworkAccessManager::finished, this, &QQLyricFetcher::lyricGetFinished);
}

void QQLyricFetcher::lyricGetFinished(QNetworkReply *reply) {

}

void QQLyricFetcher::songGetFinished(QNetworkReply *reply) {
    std::cout << reply->request().url().toString().toStdString() << std::endl;
    std::cout << reply->error() << std::endl;
    if (reply->error() == QNetworkReply::NoError) {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (statusCode.isValid()) {
            QString body(reply->readAll());
            QJsonObject jsonResponse = QJsonDocument::fromJson(body.toUtf8()).object();
            std::cout << "code: " << jsonResponse["code"].toInt() << std::endl;
            if (jsonResponse["code"].toInt() == 0) {
                QJsonArray songArray = jsonResponse["data"].toObject()["song"].toObject()["list"].toArray();
                if (!songArray.empty()) {
                    std::cout << "media_mid: " << songArray[0].toObject()["media_mid"].toString().toStdString() << std::endl;
                }
            }
        }
    }
}
