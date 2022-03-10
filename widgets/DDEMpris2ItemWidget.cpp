#include <iostream>
#include "DDEMpris2ItemWidget.h"
#include "ui_DDEMpris2ItemWidget.h"

DDEMpris2ItemWidget::DDEMpris2ItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDEMpris2ItemWidget)
{
    ui->setupUi(this);
    this->setThemeIcon(false);

    ui->entryLabel->hide();
    ui->entryLabel->installEventFilter(this);
    ui->entryLabel->setScaledContents(true);

    connect(ui->prevButton, &QToolButton::clicked, this, [this] () { Q_EMIT prevClicked(); });
    connect(ui->playPauseButton, &QToolButton::clicked, this, [this] () { Q_EMIT playPauseClicked(); });
    connect(ui->nextButton, &QToolButton::clicked, this, [this] () { Q_EMIT nextClicked(); });
}

DDEMpris2ItemWidget::~DDEMpris2ItemWidget()
{
    delete ui;
}

void DDEMpris2ItemWidget::setTextAlign(Qt::AlignmentFlag align) {
    ui->label->setAlignment(align);
}

void DDEMpris2ItemWidget::setText(QString text) {
    ui->label->setText(text);
}

void DDEMpris2ItemWidget::setPlayblackStatus(bool isPlaying) {
    if (isPlaying) {
        if (!this->dark) {
            ui->playPauseButton->setIcon(QIcon(":/icons/resources/pause-black.svg"));
        } else {
            ui->playPauseButton->setIcon(QIcon(":/icons/resources/pause-light.svg"));
        }
    } else {
        if (!this->dark) {
            ui->playPauseButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
        } else {
            ui->playPauseButton->setIcon(QIcon(":/icons/resources/play-light.svg"));
        }
    }
}

void DDEMpris2ItemWidget::setDesktopEntry(QString entry) {
    if (entry == this->currEntry) {
        return;
    }

    this->currEntry = entry;
    if (this->currEntry.isEmpty()) {
        ui->entryLabel->hide();
    } else {
        ui->entryLabel->show();
        QIcon icon = QIcon::fromTheme(this->currEntry);
        QPixmap pixmap = icon.pixmap(ui->entryLabel->sizeHint() * ui->entryLabel->devicePixelRatioF());
        pixmap.setDevicePixelRatio(ui->entryLabel->devicePixelRatioF());
        ui->entryLabel->setPixmap(pixmap);
    }
}

bool DDEMpris2ItemWidget::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->entryLabel) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto *mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                Q_EMIT entryClicked();
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return QWidget::eventFilter(obj, event);
    }
}

QString DDEMpris2ItemWidget::text() {
    return ui->label->text().trimmed();
}

void DDEMpris2ItemWidget::setText(QString text, qlonglong t) {
    ui->label->setText(QString("  %1  ").arg(text), t);
}

void DDEMpris2ItemWidget::hideDesktopEntry() {
    ui->entryLabel->hide();
}

void DDEMpris2ItemWidget::setFontColor(QColor color) {
    ui->label->setFontColor(color);
}

void DDEMpris2ItemWidget::setThemeIcon(bool dark) {
    this->dark = dark;
    if (dark) {
        ui->prevButton->setIcon(QIcon(":/icons/resources/prev-light.svg"));
        ui->nextButton->setIcon(QIcon(":/icons/resources/next-light.svg"));
        ui->playPauseButton->setIcon(QIcon(":/icons/resources/play-light.svg"));
    } else {
        ui->prevButton->setIcon(QIcon(":/icons/resources/prev-black.svg"));
        ui->nextButton->setIcon(QIcon(":/icons/resources/next-black.svg"));
        ui->playPauseButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
    }
}
