#ifndef SCROLLLABEL_H
#define SCROLLLABEL_H

#include <QLabel>
#include <QTimer>
#include <QTimerEvent>
#include <QPaintEvent>

class ScrollLabel : public QLabel
{
public:
    ScrollLabel(QWidget *parent = nullptr);
    void setText(const QString &text);
    void setText(const QString &text, qlonglong timeInUs);
    void setFontColor(QColor color);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void checkStr();
    void scrollTimerEvent();

    QTimer *scrollTimer_p;
    double offset;
    QString spaceStr;

    qlonglong drawTimeLength;

    QColor fontColor = Qt::black;
};

#endif // SCROLLLABEL_H
