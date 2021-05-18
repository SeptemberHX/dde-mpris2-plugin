//
// Created by ragdoll on 2021/5/18.
//

#ifndef DDE_MPRIS2_QQLYRICFETCHER_H
#define DDE_MPRIS2_QQLYRICFETCHER_H

#include "AbstractLyricFetcher.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "MLyric.h"

class QQLyricFetcher : public AbstractLyricFetcher {

public:
    QQLyricFetcher();

    void requestForLyric(QString title, QString artist, QString album) override;

    static QString searchUrl;
    static QString lyricUrl;

private slots:
    void songGetFinished(QNetworkReply *replay);

private:
    QNetworkAccessManager *naManager;
};


#endif //DDE_MPRIS2_QQLYRICFETCHER_H
