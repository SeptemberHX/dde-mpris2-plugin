//
// Created by ragdoll on 2021/5/13.
//

#ifndef DDE_MPRIS2_DBUSMONITOR_H
#define DDE_MPRIS2_DBUSMONITOR_H

#include <QString>
#include <QObject>
#include "dbusinterface.h"

class DBusMonitor : public QObject {

    Q_OBJECT

public:
    explicit DBusMonitor(QString servicePrefix);
    void init();

public slots:
    void dbusChanged(const QString &name, const QString &oldOwner, const QString &newOwner);

private:
    QString svcPrefix;
    DBusInterface *p_interface;

signals:
    void ownerLost(QString name);
    void ownerChanged(QString name);
};


#endif //DDE_MPRIS2_DBUSMONITOR_H
