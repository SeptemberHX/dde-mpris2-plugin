#ifndef PLAYERSTATUS_H
#define PLAYERSTATUS_H

#include <QString>

class PlayerStatus
{
public:
    PlayerStatus();

    QString getPlaybackStatus() const;
    void setPlaybackStatus(QString value);

    QString getLoopStatus() const;
    void setLoopStatus(QString value);

    QString getArtUrl() const;
    void setArtUrl(const QString &value);

    long long getLength() const;
    void setLength(long long value);

    QString getAlbum() const;
    void setAlbum(const QString &value);

    QString getArtist() const;
    void setArtist(const QString &value);

    QString getTitle() const;
    void setTitle(const QString &value);

    int getTrackNumber() const;
    void setTrackNumber(int value);

    long long getPosition() const;
    void setPosition(long long value);

    double getRate() const;
    void setRate(double value);

    bool getShuffle() const;
    void setShuffle(bool value);

    bool getCanGoNext() const;
    void setCanGoNext(bool value);

    bool getCanGoPrevious() const;
    void setCanGoPrevious(bool value);

    bool getCanPlay() const;
    void setCanPlay(bool value);

    bool getCanPause() const;
    void setCanPause(bool value);

    bool getCanSeek() const;
    void setCanSeek(bool value);

    bool getCanControl() const;
    void setCanControl(bool value);

private:
    QString playbackStatus;
    QString loopStatus;

    QString artUrl;
    long long length;
    QString album;
    QString artist;
    QString title;
    int trackNumber;

    long long position;
    double rate;
    bool shuffle;
    bool canGoNext;
    bool canGoPrevious;
    bool canPlay;
    bool canPause;
    bool canSeek;
    bool canControl;
};

#endif // PLAYERSTATUS_H
