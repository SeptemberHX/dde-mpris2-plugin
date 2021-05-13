//
// Created by septemberhx on 2020/5/13.
//

#include "DDEMpris2Plugin.h"
#include <QJsonObject>
#include <QDebug>
#include <iostream>

#define PLUGIN_STATE_KEY "enable"

DDEMpris2Plugin::DDEMpris2Plugin(QObject *parent) : QObject(parent) {
    this->p_label = new ScrollLabel();
    this->p_label->setFixedWidth(200);
    this->p_label->setText("这是一个非常非常长的句子，哈哈");

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
    return QStringLiteral("DDE Istate Menus");
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
    this->p_label->setText(name);
}

void DDEMpris2Plugin::mprisLost(QString name) {

}
