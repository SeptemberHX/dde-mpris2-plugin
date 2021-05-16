#ifndef DDEMPRIS2WIDGET_H
#define DDEMPRIS2WIDGET_H

#include <QWidget>
#include "dbus/PlayerStatus.h"

namespace Ui {
class DDEMpris2Widget;
}

class DDEMpris2Widget : public QWidget
{
    Q_OBJECT

public:
    explicit DDEMpris2Widget(QWidget *parent = nullptr);
    ~DDEMpris2Widget();

    void showStatus(PlayerStatus status);
    void updatePosition(qlonglong position);

    const PlayerStatus& getStatus();
    void setPlayPauseStatus(bool isPlaying);

signals:
    void prevClicked();
    void nextClicked();
    void pausePlayClicked();

private:
    Ui::DDEMpris2Widget *ui;
    PlayerStatus currStatus;
};

#endif // DDEMPRIS2WIDGET_H
