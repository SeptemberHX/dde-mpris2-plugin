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
    void resetToDefault();

    const PlayerStatus& getStatus();
    void setPlayPauseStatus(bool isPlaying);

    void setFontColor(QColor color);
    void setThemeIcon(bool dark);

signals:
    void prevClicked();
    void nextClicked();
    void pausePlayClicked();

private:
    Ui::DDEMpris2Widget *ui;
    PlayerStatus currStatus;

    bool isDefault;
    bool dark;
};

#endif // DDEMPRIS2WIDGET_H
