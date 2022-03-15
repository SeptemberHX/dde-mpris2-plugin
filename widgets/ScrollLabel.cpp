#include "ScrollLabel.h"
#include <QPainter>
#include <QRect>
#include <iostream>

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
        QFontMetrics fm(this->font());
        int spaceWidth = 0;
        if (this->drawTimeLength == 0) {
            spaceWidth += fm.width(this->spaceStr);
        }

        int textLength = fm.width(text());
        if (this->drawTimeLength == 0) {
            this->offset += 1;
        } else {
            this->offset += (textLength - this->width()) * 1.0 / (this->drawTimeLength * 1.0 / 1000 / 33);
        }

        if (textLength + spaceWidth < this->offset) {
            if (this->drawTimeLength == 0) {
                this->offset = 0;
            }
        }

        if (this->drawTimeLength != 0 && this->offset > textLength - this->width()) {
            this->offset = textLength - this->width();
        }
        this->repaint();
        return;
    }
}

void ScrollLabel::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QPen(this->fontColor));

    QRect rect = this->rect();
    rect.setLeft(rect.left() - offset);

    QString textStr = this->text();
    if (offset > 0 && this->drawTimeLength == 0) {
        textStr += this->spaceStr + text();
    }

    int flags = Qt::AlignCenter;
    if (this->scrollTimer_p->isActive()) {
        flags = Qt::AlignLeft | Qt::AlignVCenter;
    }
    painter.drawText(rect, flags, textStr);
}

void ScrollLabel::checkStr() {
    QFontMetrics fm(this->font());

    if (fm.width(text()) < this->width()) {
        this->offset = 0;
        this->scrollTimer_p->stop();
    } else {
        // about 30 fps
        this->scrollTimer_p->start(33);
    }
}

void ScrollLabel::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    this->checkStr();
}

void ScrollLabel::setText(const QString &text) {
    this->setText(text, 0);
}

void ScrollLabel::setText(const QString &text, qlonglong timeInUs) {
    if (timeInUs < 0) timeInUs = 0;

    if (text == this->text() && timeInUs == this->drawTimeLength) {
        return;
    }

    QLabel::setText(text);
    this->offset = 0;
    this->drawTimeLength = timeInUs;
    this->checkStr();
}

void ScrollLabel::setFontColor(QColor color) {
    this->fontColor = color;
    this->repaint();
}
