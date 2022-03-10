//
// Created by septemberhx on 2020/5/13.
//

#include "DDEMpris2Plugin.h"
#include <QJsonObject>
#include <QDebug>
#include <iostream>
#include <QTextDocument>

#define PLUGIN_STATE_KEY "enable"

DDEMpris2Plugin::DDEMpris2Plugin(QObject *parent) : QObject(parent) {
    this->defaultStr = "让音乐开始, 让节奏不停";
    this->currPlayer = nullptr;

    this->lyricFetcher = new QQLyricFetcher();
    connect(this->lyricFetcher, &AbstractLyricFetcher::lyricFetched, this, &DDEMpris2Plugin::lyricFetched);

    this->p_itemWidget = new DDEMpris2ItemWidget();
    this->p_itemWidget->setFixedWidth(250);
    this->p_itemWidget->setTextAlign(Qt::AlignCenter);
    this->p_itemWidget->setText(this->defaultStr);
    connect(this->p_itemWidget, &DDEMpris2ItemWidget::prevClicked, this, &DDEMpris2Plugin::prev);
    connect(this->p_itemWidget, &DDEMpris2ItemWidget::playPauseClicked, this, &DDEMpris2Plugin::playPause);
    connect(this->p_itemWidget, &DDEMpris2ItemWidget::nextClicked, this, &DDEMpris2Plugin::next);
    connect(this->p_itemWidget, &DDEMpris2ItemWidget::entryClicked, this, [this] () {
        if (this->currPlayer != nullptr) {
            this->currPlayer->raise();
        }
    });

    this->p_mpris2Widget = new DDEMpris2Widget();
    connect(this->p_mpris2Widget, &DDEMpris2Widget::prevClicked, this, &DDEMpris2Plugin::prev);
    connect(this->p_mpris2Widget, &DDEMpris2Widget::pausePlayClicked, this, &DDEMpris2Plugin::playPause);
    connect(this->p_mpris2Widget, &DDEMpris2Widget::nextClicked, this, &DDEMpris2Plugin::next);

    this->posTimer_p = new QTimer(this);
    connect(this->posTimer_p, &QTimer::timeout, this, [this]() {
        // maybe it is not so efficient here, it is the most easiest way to get the position
        //  since no signal is submitted when position is changed
        // After check how many dbus calls show when I move my mouse, I decide to use this implement.
        //  and Seeked signal is also not needed anymore.
        if (this->currPlayer != nullptr) {
            PlayerStatus status = this->currPlayer->playerStatus();
//            this->p_mpris2Widget->updatePosition(status.getPosition());
            this->showLyric(status.getPosition());
            this->p_mpris2Widget->showStatus(status);

            // some applications do not update playback status after pause/play, this is a trick to fix this.
            if (this->samePosCount < 5) {
                this->playbackStatusChanged(status.getPlaybackStatus());
            }
            if (this->currPos == status.getPosition()) {
                ++this->samePosCount;
                if (this->samePosCount >= 5) {
                    this->playbackStatusChanged("Paused");
                    this->samePosCount = 5;
                }
            } else {
                this->samePosCount = 0;
            }
            this->currPos = status.getPosition();
        } else {
            this->p_mpris2Widget->resetToDefault();
        }
    });

    this->p_mprisMonitor = new DBusMonitor("org.mpris.MediaPlayer2.");
    connect(this->p_mprisMonitor, &DBusMonitor::ownerChanged, this, &DDEMpris2Plugin::mprisAccqired);
    connect(this->p_mprisMonitor, &DBusMonitor::ownerLost, this, &DDEMpris2Plugin::mprisLost);
    this->p_mprisMonitor->init();

    this->posTimer_p->start(200);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [this] () {
        this->setTheme(DGuiApplicationHelper::instance()->themeType());
    });
    this->setTheme(DGuiApplicationHelper::instance()->themeType());
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
    return this->p_itemWidget;
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
        this->p_itemWidget->setText(this->defaultStr);
    } else {
        if (status.getTitle() != this->p_mpris2Widget->getStatus().getTitle() && status.getArtist() != this->p_mpris2Widget->getStatus().getArtist()) {
            this->currLyric = MLyric();
            this->lyricFetcher->requestForLyric(status.getTitle(), status.getArtist(), status.getAlbum());
            this->p_itemWidget->setText(status.getTitle() + " - " + status.getArtist());
            this->p_mpris2Widget->showStatus(status);
        }
    }
    this->playbackStatusChanged(this->currPlayer->playbackStatus());
    this->p_itemWidget->setDesktopEntry(this->currPlayer->desktopEntry());
}

void DDEMpris2Plugin::resetStatus() {
    this->p_itemWidget->setText(this->defaultStr);
    this->p_mpris2Widget->resetToDefault();
    this->p_itemWidget->hideDesktopEntry();
    this->currPlayer = nullptr;
}

QWidget *DDEMpris2Plugin::itemPopupApplet(const QString &itemKey) {
    Q_UNUSED(itemKey)
    return this->p_mpris2Widget;
}

void DDEMpris2Plugin::playbackStatusChanged(QString status) {
    bool isPlaying = status == "Playing";
    this->p_mpris2Widget->setPlayPauseStatus(isPlaying);
    this->p_itemWidget->setPlayblackStatus(isPlaying);
}

void DDEMpris2Plugin::prev() {
    if (this->currPlayer != nullptr) {
        this->currPlayer->playPrev();
    }
}

void DDEMpris2Plugin::next() {
    if (this->currPlayer != nullptr) {
        this->currPlayer->playNext();
    }
}

void DDEMpris2Plugin::playPause() {
    if (this->currPlayer != nullptr) {
        this->currPlayer->playPause();
    }
}

void DDEMpris2Plugin::lyricFetched(MLyric lyric) {
    this->currLyric = lyric;
}

void DDEMpris2Plugin::showLyric(qlonglong t) {
    if (!this->currLyric.isEmpty()) {
        QString str = this->currLyric.getByTime(t);
        if (this->p_itemWidget->text() != str && !str.isEmpty()) {
            auto r = this->currLyric.getWithTimeRange(t);
            QTextDocument text;
            text.setHtml(r.second);
            this->p_itemWidget->setText(text.toPlainText(), r.first.second - r.first.first);
        }
    }
}

qlonglong DDEMpris2Plugin::getCurrPos() const {
    return currPos;
}

void DDEMpris2Plugin::setTheme(DGuiApplicationHelper::ColorType theme) {
    switch (theme) {
        case Dtk::Gui::DGuiApplicationHelper::DarkType:
            this->p_itemWidget->setFontColor(Qt::white);
            this->p_mpris2Widget->setFontColor(Qt::white);
            this->p_itemWidget->setThemeIcon(true);
            this->p_mpris2Widget->setThemeIcon(true);
            break;
        case Dtk::Gui::DGuiApplicationHelper::LightType:
            this->p_itemWidget->setFontColor(Qt::black);
            this->p_mpris2Widget->setFontColor(Qt::black);
            this->p_itemWidget->setThemeIcon(false);
            this->p_mpris2Widget->setThemeIcon(false);
            break;
        default:
            break;
    }
}
