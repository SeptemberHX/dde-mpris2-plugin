//
// Created by ragdoll on 2021/5/18.
//

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include "QQLyricFetcher.h"

QString QQLyricFetcher::searchUrl("https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w=%1&format=json");
QString QQLyricFetcher::lyricUrl("https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid=%1&musicid=%2&format=json&nobase64=1");

void QQLyricFetcher::requestForLyric(QString title, QString artist, QString album) {
    QNetworkRequest request;
    QString s = title + ' ' + artist;
    if (album != title) {
        s += ' ' + album;
    }
    QUrl url(searchUrl.arg(s));
    request.setUrl(url);
    this->naManager->get(request);
}

QQLyricFetcher::QQLyricFetcher() {
    this->naManager = new QNetworkAccessManager(this);
    connect(this->naManager, &QNetworkAccessManager::finished, this, &QQLyricFetcher::songGetFinished);
}


void QQLyricFetcher::songGetFinished(QNetworkReply *reply) {
    std::cout << reply->request().url().toString().toStdString() << std::endl;
    if (reply->error() == QNetworkReply::NoError) {
        if (reply->request().url().toString().startsWith("https://c.y.qq.com/soso/fcgi-bin/client_search_cp")) {
            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            if (statusCode.isValid()) {
                QString body(reply->readAll());
                QJsonObject jsonResponse = QJsonDocument::fromJson(body.toUtf8()).object();
                std::cout << "code: " << jsonResponse["code"].toInt() << std::endl;
                if (jsonResponse["code"].toInt() == 0) {
                    QJsonArray songArray = jsonResponse["data"].toObject()["song"].toObject()["list"].toArray();
                    if (!songArray.empty()) {
                        std::cout << "songmid: " << songArray[0].toObject()["songmid"].toString().toStdString()
                                  << std::endl;
                    }

                    // request for lyric by media_mid
                    QUrl url(lyricUrl.arg(songArray[0].toObject()["songmid"].toString(), songArray[0].toObject()["songid"].toString()));
                    QNetworkRequest request(url);
                    request.setRawHeader("Referer", "https://y.qq.com/portal/player.html");
                    this->naManager->get(request);
                }
            }
        } else {
            QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            if (statusCode.isValid()) {
                QString body(reply->readAll());
                QJsonObject jsonResponse = QJsonDocument::fromJson(body.toUtf8()).object();
                std::cout << "code: " << jsonResponse["code"].toInt() << std::endl;
                if (jsonResponse["code"].toInt() == 0) {
                    QString lyric = jsonResponse["lyric"].toString();
                    MLyric formatedLyric(lyric);
                    Q_EMIT lyricFetched(formatedLyric);
                }
            }
        }
    }
}
