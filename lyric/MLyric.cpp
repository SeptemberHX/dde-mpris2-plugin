//
// Created by ragdoll on 2021/5/18.
//

#include <iostream>
#include "MLyric.h"

MLyric::MLyric(QString lyricText) {
    foreach (QString line, lyricText.split('\n')) {
        if (line.startsWith("[ti:")) {
            this->title = line.section(':', 1).remove(']');
        } else if (line.startsWith("[ar:")) {
            this->artist = line.section(':', 1).remove(']');
        } else if (line.startsWith("[by:")) {
            this->by = line.section(':', 1).remove(']');
        } else if (line.startsWith("[offset:")) {
            this->offset = line.section(':', 1).remove(']').toInt();
        } else if (line.contains(QRegExp(R"(\[[0-9]{1,2}:[0-9]{1,2}\.[0-9]{1,2}\])"))) {
            int min = line.mid(1, 2).toInt();
            int sec = line.mid(4, 2).toInt();
            int msec = line.mid(7, 2).toInt();
            qlonglong t = ((min * 60 + sec) * 1000 + msec) * 1000;
            QString lyric = line.section(']', 1);
            this->lyricWithTime.append(QPair<qlonglong, QString>(t, lyric));
        }
    }
}

void MLyric::print() {
    std::cout << this->title.toStdString() << " " << this->artist.toStdString() << " " << this->offset << std::endl;
    for (auto iter = this->lyricWithTime.begin(); iter != this->lyricWithTime.end(); ++iter) {
        std::cout << iter->first << " " << iter->second.toStdString() << std::endl;
    }
}

QString MLyric::getByTime(qlonglong t) {
    for (int i = 0; i < this->lyricWithTime.size() - 1; ++i) {
        if (this->lyricWithTime[i].first <= t and this->lyricWithTime[i+1].first > t) {
            return this->lyricWithTime[i].second;
        }
    }
    return this->lyricWithTime.last().second;
}

bool MLyric::isEmpty() {
    return this->lyricWithTime.isEmpty();
}

QPair<QPair<qlonglong, qlonglong>, QString> MLyric::getWithTimeRange(qlonglong t) {
    qlonglong t1 = -1, t2 = -1;
    for (int i = 0; i < this->lyricWithTime.size() - 1; ++i) {
        if (this->lyricWithTime[i].first <= t and this->lyricWithTime[i+1].first > t) {
            t1 = this->lyricWithTime[i].first;
            t2 = this->lyricWithTime[i+1].first;
            return QPair<QPair<qlonglong, qlonglong>, QString>(QPair<qlonglong, qlonglong>(t1, t2), this->lyricWithTime[i].second);
        }
    }
    t1 = this->lyricWithTime.last().first;
    return QPair<QPair<qlonglong, qlonglong>, QString>(QPair<qlonglong, qlonglong>(t1, t2), this->lyricWithTime.last().second);
}
