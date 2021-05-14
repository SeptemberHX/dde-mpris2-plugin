#include "PlayerStatus.h"

PlayerStatus::PlayerStatus()
{
    this->rate = 1.0;
}

QString PlayerStatus::getPlaybackStatus() const
{
    return playbackStatus;
}

void PlayerStatus::setPlaybackStatus(QString value)
{
    playbackStatus = value;
}

QString PlayerStatus::getLoopStatus() const
{
    return loopStatus;
}

void PlayerStatus::setLoopStatus(QString value)
{
    loopStatus = value;
}

QString PlayerStatus::getArtUrl() const
{
    return artUrl;
}

void PlayerStatus::setArtUrl(const QString &value)
{
    artUrl = value;
}

long long PlayerStatus::getLength() const
{
    return length;
}

void PlayerStatus::setLength(long long value)
{
    length = value;
}

QString PlayerStatus::getAlbum() const
{
    return album;
}

void PlayerStatus::setAlbum(const QString &value)
{
    album = value;
}

QString PlayerStatus::getArtist() const
{
    return artist;
}

void PlayerStatus::setArtist(const QString &value)
{
    artist = value;
}

QString PlayerStatus::getTitle() const
{
    return title;
}

void PlayerStatus::setTitle(const QString &value)
{
    title = value;
}

int PlayerStatus::getTrackNumber() const
{
    return trackNumber;
}

void PlayerStatus::setTrackNumber(int value)
{
    trackNumber = value;
}

long long PlayerStatus::getPosition() const
{
    return position;
}

void PlayerStatus::setPosition(long long value)
{
    position = value;
}

double PlayerStatus::getRate() const
{
    return rate;
}

void PlayerStatus::setRate(double value)
{
    rate = value;
}

bool PlayerStatus::getShuffle() const
{
    return shuffle;
}

void PlayerStatus::setShuffle(bool value)
{
    shuffle = value;
}

bool PlayerStatus::getCanGoNext() const
{
    return canGoNext;
}

void PlayerStatus::setCanGoNext(bool value)
{
    canGoNext = value;
}

bool PlayerStatus::getCanGoPrevious() const
{
    return canGoPrevious;
}

void PlayerStatus::setCanGoPrevious(bool value)
{
    canGoPrevious = value;
}

bool PlayerStatus::getCanPlay() const
{
    return canPlay;
}

void PlayerStatus::setCanPlay(bool value)
{
    canPlay = value;
}

bool PlayerStatus::getCanPause() const
{
    return canPause;
}

void PlayerStatus::setCanPause(bool value)
{
    canPause = value;
}

bool PlayerStatus::getCanSeek() const
{
    return canSeek;
}

void PlayerStatus::setCanSeek(bool value)
{
    canSeek = value;
}

bool PlayerStatus::getCanControl() const
{
    return canControl;
}

void PlayerStatus::setCanControl(bool value)
{
    canControl = value;
}
