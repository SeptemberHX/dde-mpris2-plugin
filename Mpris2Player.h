#ifndef MPRIS2PLAYER_H
#define MPRIS2PLAYER_H

#include <QObject>
#include <QVariantMap>
#include <QString>
#include <QStringList>
#include "mpris.h"
#include "PlayerStatus.h"

class Mpris2Player : public QObject
{
    Q_OBJECT
public:
    explicit Mpris2Player(QString service, QObject *parent = nullptr);
    PlayerStatus playerStatus();

signals:
    void playStatusChanged(int);
    void metadataChanged();
    void seeked(qlonglong);

public slots:
    void propertyChanged(QString name, QVariantMap map, QStringList list);
    void positionSeeked(qlonglong p);

private:
    OrgMprisMediaPlayer2PlayerInterface *netease;
};

#endif // MPRIS2PLAYER_H
