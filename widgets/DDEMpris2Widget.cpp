#include "DDEMpris2Widget.h"
#include "ui_DDEMpris2Widget.h"
#include <QImage>
#include <QUrl>
#include <QPixmap>

DDEMpris2Widget::DDEMpris2Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDEMpris2Widget),
    isDefault(false)
{
    ui->setupUi(this);

    connect(ui->prevButton, &QToolButton::clicked, this, [this] () { Q_EMIT prevClicked(); });
    connect(ui->pausePlayButton, &QToolButton::clicked, this, [this] () { Q_EMIT pausePlayClicked(); });
    connect(ui->nextButton, &QToolButton::clicked, this, [this] () { Q_EMIT nextClicked(); });

    this->setThemeIcon(false);
    ui->artLabel->setScaledContents(true);

    this->resetToDefault();
}

DDEMpris2Widget::~DDEMpris2Widget()
{
    delete ui;
}

void DDEMpris2Widget::showStatus(PlayerStatus status) {
    this->isDefault = false;
    this->currStatus = status;

    ui->titleLabel->setText(this->currStatus.getTitle());
    ui->artistLabel->setText(this->currStatus.getArtist());
    ui->albumLabel->setText(this->currStatus.getAlbum());

    QUrl url = this->currStatus.getArtUrl();
    if (url.isValid()) {
        QImage image(url.path());
        if (image.isNull()) {
            image = QImage(":/icons/resources/disc.svg");
        }

        QPixmap pixmap = QPixmap::fromImage(image);
        pixmap.setDevicePixelRatio(ui->artLabel->devicePixelRatioF());
        ui->artLabel->setPixmap(QPixmap::fromImage(image));
    }
    this->updatePosition(status.getPosition());

    ui->nextButton->setEnabled(status.getCanGoNext());
    ui->prevButton->setEnabled(status.getCanGoPrevious());
    ui->pausePlayButton->setEnabled(status.getCanPlay() && status.getCanPause());
}

void DDEMpris2Widget::updatePosition(qlonglong position) {
    if (this->currStatus.getLength() == 0) {
        ui->progressWidget->setDisabled(true);
        ui->leftLabel->clear();
        ui->posLabel->clear();
        ui->progressBar->setValue(0);
        return;
    } else {
        ui->progressWidget->setEnabled(true);
    }

    qlonglong leftTime = this->currStatus.getLength() - position;
    ui->leftLabel->setText(QString("-%1:%2").arg(leftTime / 1000000 / 60).arg(leftTime / 1000000 % 60, 2, 10, QLatin1Char('0')));
    ui->posLabel->setText(QString("%1:%2").arg(
            position / 1000000 / 60).arg(position / 1000000 % 60, 2, 10, QLatin1Char('0')));
    ui->progressBar->setValue((int) (position * 1.0 / this->currStatus.getLength() * 100));
}

const PlayerStatus& DDEMpris2Widget::getStatus() {
    return this->currStatus;
}

void DDEMpris2Widget::setPlayPauseStatus(bool isPlaying) {
    if (!isPlaying) {
        if (!this->dark) {
            ui->pausePlayButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
        } else {
            ui->pausePlayButton->setIcon(QIcon(":/icons/resources/play-light.svg"));
        }
    } else {
        if (!this->dark) {
            ui->pausePlayButton->setIcon(QIcon(":/icons/resources/pause-black.svg"));
        } else {
            ui->pausePlayButton->setIcon(QIcon(":/icons/resources/pause-light.svg"));
        }
    }
}

void DDEMpris2Widget::resetToDefault() {
    if (!this->isDefault) {
        ui->titleLabel->setText("");
        ui->artistLabel->setText("");
        ui->albumLabel->setText(tr("没有音乐播放器"));

        QImage image(":/icons/resources/disc.svg");
        QPixmap pixmap = QPixmap::fromImage(image);
        pixmap.setDevicePixelRatio(ui->artLabel->devicePixelRatioF());
        ui->artLabel->setPixmap(QPixmap::fromImage(image));

        this->updatePosition(0);
        ui->leftLabel->setText(0);

        ui->nextButton->setEnabled(false);
        ui->prevButton->setEnabled(false);
        ui->pausePlayButton->setEnabled(false);
        this->isDefault = true;
    }
}

void DDEMpris2Widget::setFontColor(QColor color) {
    ui->albumLabel->setFontColor(color);
    ui->artistLabel->setFontColor(color);
    ui->titleLabel->setFontColor(color);
}

void DDEMpris2Widget::setThemeIcon(bool dark) {
    this->dark = dark;
    if (dark) {
        ui->prevButton->setIcon(QIcon(":/icons/resources/prev-light.svg"));
        ui->nextButton->setIcon(QIcon(":/icons/resources/next-light.svg"));
        ui->pausePlayButton->setIcon(QIcon(":/icons/resources/play-light.svg"));
    } else {
        ui->prevButton->setIcon(QIcon(":/icons/resources/prev-black.svg"));
        ui->nextButton->setIcon(QIcon(":/icons/resources/next-black.svg"));
        ui->pausePlayButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
    }
}
