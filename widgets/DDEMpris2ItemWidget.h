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
    void setText(QString text, qlonglong t);
    QString text();

    void setPlayblackStatus(bool isPlaying);
    void setDesktopEntry(QString entry);
    void hideDesktopEntry();
    void setFontColor(QColor color);
    void setThemeIcon(bool dark);

signals:
    void prevClicked();
    void playPauseClicked();
    void nextClicked();
    void entryClicked();

private:
    bool eventFilter(QObject *obj, QEvent *event) override;

    Ui::DDEMpris2ItemWidget *ui;
    QString currEntry;

    bool dark;
};

#endif // DDEMPRIS2ITEMWIDGET_H
