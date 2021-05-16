#include <iostream>
#include "DDEMpris2ItemWidget.h"
#include "ui_DDEMpris2ItemWidget.h"

DDEMpris2ItemWidget::DDEMpris2ItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDEMpris2ItemWidget)
{
    ui->setupUi(this);

    ui->prevButton->setIcon(QIcon(":/icons/resources/prev-black.svg"));
    ui->nextButton->setIcon(QIcon(":/icons/resources/next-black.svg"));
    ui->playPauseButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
    ui->entryLabel->hide();
    ui->entryLabel->installEventFilter(this);

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
        ui->playPauseButton->setIcon(QIcon(":/icons/resources/pause-black.svg"));
    } else {
        ui->playPauseButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
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
        ui->entryLabel->setPixmap(QIcon::fromTheme(this->currEntry).pixmap(ui->entryLabel->sizeHint()));
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
