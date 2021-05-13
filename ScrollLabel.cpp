#include "ScrollLabel.h"
#include <QPainter>
#include <QRect>

ScrollLabel::ScrollLabel(QWidget *parent)
    : QLabel(parent)
    , offset(0)
    , spaceStr("          ")
{
    this->scrollTimer_p = new QTimer(this);
    connect(this->scrollTimer_p, &QTimer::timeout, this, &ScrollLabel::scrollTimerEvent);
}

void ScrollLabel::scrollTimerEvent() {
    if (this->isVisible()) {
        this->offset += 2;

        QFontMetrics fm(this->font());
        if (fm.width(text()) + fm.width(this->spaceStr) < this->offset) {
            this->offset = 0;
        }
        this->repaint();
        return;
    }
}

void ScrollLabel::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black));

    QRect rect = this->rect();
    rect.setLeft(rect.left() - offset);

    QString textStr = this->text();
    if (offset > 0) {
        textStr += this->spaceStr + text();
    }
    painter.drawText(rect, Qt::AlignVCenter, textStr);
}

void ScrollLabel::checkStr() {
    QFontMetrics fm(this->font());

    if (fm.width(text()) < this->width()) {
        this->offset = 0;
        this->scrollTimer_p->stop();
    } else {
        this->scrollTimer_p->start(200);
    }
}

void ScrollLabel::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    this->checkStr();
}

void ScrollLabel::setText(const QString &text) {
    QLabel::setText(text);

    this->checkStr();
}
