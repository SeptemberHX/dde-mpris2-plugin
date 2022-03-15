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

    this->playerInterface = new OrgMprisMediaPlayer2PlayerInterface(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);
    this->interface = new OrgMprisMediaPlayer2Interface(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);
}

PlayerStatus Mpris2Player::playerStatus()
{
    PlayerStatus status;
    status.setPlaybackStatus(this->playerInterface->playbackStatus());
    status.setLoopStatus(this->playerInterface->loopStatus());
    status.setPosition(this->playerInterface->position());
    status.setRate(this->playerInterface->rate());
    status.setShuffle(this->playerInterface->shuffle());
    status.setCanGoNext(this->playerInterface->canGoNext());
    status.setCanGoPrevious(this->playerInterface->canGoPrevious());
    status.setCanPlay(this->playerInterface->canPlay());
    status.setCanPause(this->playerInterface->canPause());
    status.setCanSeek(this->playerInterface->canSeek());
    status.setCanControl(this->playerInterface->canControl());

    QVariantMap metadataMap = this->playerInterface->metadata();
    if (metadataMap.contains("mpris:artUrl")) {
        status.setArtUrl(metadataMap.value("mpris:artUrl").toString());
    }

    if (metadataMap.contains("mpris:length")) {
        status.setLength(metadataMap.value("mpris:length").toLongLong());
    } else {
        status.setLength(0);
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
    return this->playerInterface->position();
}

void Mpris2Player::propertyChanged(QString name, QVariantMap map, QStringList list)
{
    if (map.contains("PlaybackStatus")) {
//        std::cout << map.value("PlaybackStatus").toString().toStdString() << std::endl;
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
//    std::cout << "=======" << std::endl;
    emit seeked(p);
}

const QString &Mpris2Player::getName() {
    return this->name;
}

void Mpris2Player::playPrev() {
    this->playerInterface->Previous();
}

void Mpris2Player::playNext() {
    this->playerInterface->Next();
}

void Mpris2Player::playPause() {
    this->playerInterface->PlayPause();
}

QString Mpris2Player::playbackStatus() {
    return this->playerInterface->playbackStatus();
}

QString Mpris2Player::desktopEntry() {
    return this->interface->desktopEntry();
}

void Mpris2Player::raise() {
    this->interface->Raise();
}

void Mpris2Player::quit() {
    this->interface->Quit();
}
