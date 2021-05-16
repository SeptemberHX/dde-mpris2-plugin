#ifndef DDEMPRIS2ITEMWIDGET_H
#define DDEMPRIS2ITEMWIDGET_H

#include <QWidget>

namespace Ui {
class DDEMpris2ItemWidget;
}

class DDEMpris2ItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DDEMpris2ItemWidget(QWidget *parent = nullptr);
    ~DDEMpris2ItemWidget();

    void setTextAlign(Qt::AlignmentFlag align);
    void setText(QString text);

    void setPlayblackStatus(bool isPlaying);

signals:
    void prevClicked();
    void playPauseClicked();
    void nextClicked();

private:
    Ui::DDEMpris2ItemWidget *ui;
};

#endif // DDEMPRIS2ITEMWIDGET_H
