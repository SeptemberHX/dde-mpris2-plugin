//
// Created by septemberhx on 2020/5/13.
//

#include "DDEMpris2Plugin.h"
#include <QJsonObject>
#include <QDebug>
#include <iostream>

#define PLUGIN_STATE_KEY "enable"

DDEMpris2Plugin::DDEMpris2Plugin(QObject *parent) : QObject(parent) {
    this->defaultStr = "让音乐开始, 让节奏不停";
    this->currPlayer = nullptr;

    this->p_label = new ScrollLabel();
    this->p_label->setAlignment(Qt::AlignHCenter);
    this->p_label->setFixedWidth(250);
    this->p_label->setText(this->defaultStr);

    this->p_mpris2Widget = new DDEMpris2Widget();
    connect(this->p_mpris2Widget, &DDEMpris2Widget::prevClicked, this, [this] () {
        if (this->currPlayer != nullptr) {
            this->currPlayer->playPrev();
        }
    });
    connect(this->p_mpris2Widget, &DDEMpris2Widget::pausePlayClicked, this, [this] () {
        if (this->currPlayer != nullptr) {
            this->currPlayer->playPause();
        }
    });
    connect(this->p_mpris2Widget, &DDEMpris2Widget::nextClicked, this, [this] () {
        if (this->currPlayer != nullptr) {
            this->currPlayer->playNext();
        }
    });

    this->posTimer_p = new QTimer(this);
    connect(this->posTimer_p, &QTimer::timeout, this, [this]() {
        // maybe it is not so efficient here, it is the most easiest way to get the position
        //  since no signal is submitted when position is changed
        // After check how many dbus calls show when I move my mouse, I decide to use this implement.
        //  and Seeked signal is also not needed anymore.
        this->p_mpris2Widget->updatePosition(this->currPlayer->position());
    });
    this->posTimer_p->start(500);

    this->p_mprisMonitor = new DBusMonitor("org.mpris.MediaPlayer2.");
    connect(this->p_mprisMonitor, &DBusMonitor::ownerChanged, this, &DDEMpris2Plugin::mprisAccqired);
    connect(this->p_mprisMonitor, &DBusMonitor::ownerLost, this, &DDEMpris2Plugin::mprisLost);
    this->p_mprisMonitor->init();
}

const QString DDEMpris2Plugin::pluginName() const {
    return QStringLiteral("dde_mpris2");
}

void DDEMpris2Plugin::init(PluginProxyInterface *proxyInter) {
    this->m_proxyInter = proxyInter;

    if (!pluginIsDisable()) {
        this->m_proxyInter->itemAdded(this, this->pluginName());
        m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, true);
    }
}

bool DDEMpris2Plugin::pluginIsAllowDisable() {
    return true;
}

bool DDEMpris2Plugin::pluginIsDisable() {
    return !(m_proxyInter->getValue(this, PLUGIN_STATE_KEY, true).toBool());
}

void DDEMpris2Plugin::pluginStateSwitched() {
    m_proxyInter->saveValue(this, PLUGIN_STATE_KEY, pluginIsDisable());

    if (pluginIsDisable()) {
        m_proxyInter->itemRemoved(this, pluginName());
        return;
    }

    m_proxyInter->itemAdded(this, pluginName());
}

const QString DDEMpris2Plugin::pluginDisplayName() const {
    return QStringLiteral("DDE Mpris2");
}

void DDEMpris2Plugin::pluginSettingsChanged() {
    if (pluginIsDisable()) {
        m_proxyInter->itemRemoved(this, pluginName());
        return;
    }

    m_proxyInter->itemAdded(this, pluginName());
}

QWidget *DDEMpris2Plugin::itemWidget(const QString &itemKey) {
    Q_UNUSED(itemKey)
    return this->p_label;
}

void DDEMpris2Plugin::mprisAccqired(QString name) {
    Mpris2Player *player = new Mpris2Player(name);
    for (auto iter = this->playerList.begin(); iter != this->playerList.end(); ++iter) {
        if ((*iter)->getName() == name) {
            this->playerList.erase(iter);
        }
    }

    if (!this->playerList.isEmpty()) {
        disconnect(this->playerList.last(), &Mpris2Player::metadataChanged, this, &DDEMpris2Plugin::metadataChanged);
        disconnect(this->playerList.last(), &Mpris2Player::playStatusChanged, this, &DDEMpris2Plugin::playbackStatusChanged);
    }
    this->playerList.append(player);
    this->setToLastPlayer();
}

void DDEMpris2Plugin::mprisLost(QString name) {
    Mpris2Player *lastPlayer = nullptr;
    for (auto iter = this->playerList.begin(); iter != this->playerList.end(); ++iter) {
        if ((*iter)->getName() == name) {
            if (iter == this->playerList.end() - 1) {
                lastPlayer = *iter;
            }
            iter = this->playerList.erase(iter);
            --iter;
        }
    }

    if (lastPlayer != nullptr) {
        disconnect(lastPlayer, &Mpris2Player::metadataChanged, this, &DDEMpris2Plugin::metadataChanged);
        disconnect(lastPlayer, &Mpris2Player::playStatusChanged, this, &DDEMpris2Plugin::playbackStatusChanged);
    }

    this->setToLastPlayer();
}

void DDEMpris2Plugin::setToLastPlayer() {
    if (this->playerList.isEmpty()) {
        this->resetStatus();
        return;
    }
    connect(this->playerList.last(), &Mpris2Player::metadataChanged, this, &DDEMpris2Plugin::metadataChanged);
    connect(this->playerList.last(), &Mpris2Player::playStatusChanged, this, &DDEMpris2Plugin::playbackStatusChanged);
    this->setPlayerStatus(this->playerList.last(), this->playerList.last()->playerStatus());
}

void DDEMpris2Plugin::metadataChanged() {
    Mpris2Player *player = dynamic_cast<Mpris2Player*>(QObject::sender());

    PlayerStatus status = player->playerStatus();
    this->setPlayerStatus(player, status);
    this->p_mpris2Widget->updatePosition(player->position());
}

void DDEMpris2Plugin::setPlayerStatus(Mpris2Player *player, PlayerStatus status) {
    this->currPlayer = player;

    if (status.getTitle().isEmpty()) {
        this->p_label->setText(this->defaultStr);
    } else {
        this->p_label->setText(status.getTitle() + " - " + status.getArtist());
        this->p_mpris2Widget->showStatus(status);
    }
}

void DDEMpris2Plugin::resetStatus() {
    this->p_label->setText(this->defaultStr);
}

QWidget *DDEMpris2Plugin::itemPopupApplet(const QString &itemKey) {
    Q_UNUSED(itemKey)
    return this->p_mpris2Widget;
}

void DDEMpris2Plugin::playbackStatusChanged(QString status) {
    std::cout << "Status: " << status.toStdString() << std::endl;
    if (status == "Playing") {
        this->posTimer_p->stop();
        this->posTimer_p->start(500);
    } else if (status == "Paused" || status == "Stopped" ) {
        this->posTimer_p->stop();
    }
}