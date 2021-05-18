//
// Created by ragdoll on 2021/5/18.
//

#ifndef DDE_MPRIS2_MLYRIC_H
#define DDE_MPRIS2_MLYRIC_H

#include <QString>
#include <QPair>
#include <QList>
#include <QMetaType>

class MLyric {

public:
    MLyric(QString lyricText);
    MLyric() = default;

    QString getByTime(qlonglong t);
    QPair<QPair<qlonglong, qlonglong>, QString> getWithTimeRange(qlonglong t);
    bool isEmpty();

    void print();

private:
    QString title;
    QString artist;
    QString album;
    QString by;
    int offset;

    // time in us
    QList<QPair<qlonglong, QString>> lyricWithTime;
};

Q_DECLARE_METATYPE(MLyric);

#endif //DDE_MPRIS2_MLYRIC_H
