//
// Created by ragdoll on 2021/5/18.
//

#ifndef DDE_MPRIS2_QQLYRICFETCHER_H
#define DDE_MPRIS2_QQLYRICFETCHER_H

#include "AbstractLyricFetcher.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

class QQLyricFetcher : public AbstractLyricFetcher {

public:
    QQLyricFetcher();

    void requestForLyric(QString title, QString artist) override;

private slots:
    void songGetFinished(QNetworkReply *replay);
    void lyricGetFinished(QNetworkReply *replay);

private:
    QNetworkAccessManager *idManager;
    QNetworkAccessManager *lyricManager;
};


#endif //DDE_MPRIS2_QQLYRICFETCHER_H
