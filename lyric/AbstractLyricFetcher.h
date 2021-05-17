//
// Created by ragdoll on 2021/5/18.
//

#ifndef DDE_MPRIS2_ABSTRACTLYRICFETCHER_H
#define DDE_MPRIS2_ABSTRACTLYRICFETCHER_H

#include <QObject>

class AbstractLyricFetcher : public QObject {

    Q_OBJECT

public:
    AbstractLyricFetcher() = default;

    virtual void requestForLyric(QString title, QString artist) = 0;

signals:
    void lyricFetched();
};

#endif //DDE_MPRIS2_ABSTRACTLYRICFETCHER_H
