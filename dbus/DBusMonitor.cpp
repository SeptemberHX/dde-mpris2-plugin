//
// Created by ragdoll on 2021/5/13.
//

#include <QDBusConnection>
#include "DBusMonitor.h"

DBusMonitor::DBusMonitor(QString servicePrefix)
    : svcPrefix(servicePrefix)
{
    this->p_interface = new DBusInterface("org.freedesktop.DBus", "/org/freedesktop/DBus", QDBusConnection::sessionBus(), this);
}

void DBusMonitor::dbusChanged(const QString &name, const QString &oldOwner, const QString &newOwner) {
    if (!name.startsWith(this->svcPrefix)) {
        return;
    }

    if (newOwner.isEmpty()) {
        Q_EMIT ownerLost(name);
    } else {
        Q_EMIT ownerChanged(name);
    }
}

void DBusMonitor::init() {
    connect(p_interface, &DBusInterface::NameOwnerChanged, this, &DBusMonitor::dbusChanged);
    for (const auto &name : p_interface->ListNames().value()) {
        this->dbusChanged(name, QString(), name);
    }
}
