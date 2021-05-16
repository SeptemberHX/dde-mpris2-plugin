#include "Mpris2Player.h"
#include <iostream>
#include <QDBusConnection>
#include <QDebug>

Mpris2Player::Mpris2Player(QString service, QObject *parent) : QObject(parent)
{
    this->name = service;
    bool r = QDBusConnection::sessionBus().connect(
                service,
                "/org/mpris/MediaPlayer2",
                "org.freedesktop.DBus.Properties",
                "PropertiesChanged",
                this,
                SLOT(propertyChanged(QString, QVariantMap, QStringList))
    );
    if (!r) {
        qDebug() << "Connection failed";
    }

    r = QDBusConnection::sessionBus().connect(
                service,
                "/org/mpris/MediaPlayer2",
                "org.mpris.MediaPlayer2.Player",
                "Seeked",
                this,
                SLOT(positionSeeked(qlonglong))
    );
    if (!r) {
        qDebug() << "Connection failed";
    }

    this->netease = new OrgMprisMediaPlayer2PlayerInterface(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);
}

PlayerStatus Mpris2Player::playerStatus()
{
    PlayerStatus status;
    status.setPlaybackStatus(this->netease->playbackStatus());
    status.setLoopStatus(this->netease->loopStatus());
    status.setPosition(this->netease->position());
    status.setRate(this->netease->rate());
    status.setShuffle(this->netease->shuffle());
    status.setCanGoNext(this->netease->canGoNext());
    status.setCanGoPrevious(this->netease->canGoPrevious());
    status.setCanPlay(this->netease->canPlay());
    status.setCanPause(this->netease->canPause());
    status.setCanSeek(this->netease->canSeek());
    status.setCanControl(this->netease->canControl());

    QVariantMap metadataMap = this->netease->metadata();
    if (metadataMap.contains("mpris:artUrl")) {
        status.setArtUrl(metadataMap.value("mpris:artUrl").toString());
    }

    if (metadataMap.contains("mpris:length")) {
        status.setLength(metadataMap.value("mpris:length").toLongLong());
    }

    if (metadataMap.contains("xesam:album")) {
        status.setAlbum(metadataMap.value("xesam:album").toString());
    }

    if (metadataMap.contains("xesam:artist")) {
        status.setArtist(metadataMap.value("xesam:artist").toString());
    }

    if (metadataMap.contains("xesam:title")) {
        status.setTitle(metadataMap.value("xesam:title").toString());
    }

    if (metadataMap.contains("xesam:trackNumber")) {
        status.setTrackNumber(metadataMap.value("xesam:trackNumber").toInt());
    }

    return status;
}

qlonglong Mpris2Player::position() {
    return this->netease->position();
}

void Mpris2Player::propertyChanged(QString name, QVariantMap map, QStringList list)
{
    if (map.contains("PlaybackStatus")) {
        std::cout << map.value("PlaybackStatus").toString().toStdString() << std::endl;
        emit playStatusChanged(map.value("PlaybackStatus").toString());
    }

    if (map.contains("Metadata")) {
        emit metadataChanged();
    }

//    std::cout << name.toStdString() << std::endl;
//    foreach (auto key, map.keys()) {
//        std::cout << key.toStdString() << " : " << map.value(key).toString().toStdString() << std::endl;
//    }
//    foreach (auto s, list) {
//        std::cout << s.toStdString() << std::endl;
//    }
}

void Mpris2Player::positionSeeked(qlonglong p)
{
    std::cout << "=======" << std::endl;
    emit seeked(p);
}

const QString &Mpris2Player::getName() {
    return this->name;
}

void Mpris2Player::playPrev() {
    this->netease->Previous();
}

void Mpris2Player::playNext() {
    this->netease->Next();
}

void Mpris2Player::playPause() {
    this->netease->PlayPause();
}

QString Mpris2Player::playbackStatus() {
    return this->netease->playbackStatus();
}
