#include "DDEMpris2Widget.h"
#include "ui_DDEMpris2Widget.h"
#include <QImage>
#include <QUrl>
#include <QPixmap>

DDEMpris2Widget::DDEMpris2Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDEMpris2Widget)
{
    ui->setupUi(this);

    connect(ui->prevButton, &QToolButton::clicked, this, [this] () { Q_EMIT prevClicked(); });
    connect(ui->pausePlayButton, &QToolButton::clicked, this, [this] () { Q_EMIT pausePlayClicked(); });
    connect(ui->nextButton, &QToolButton::clicked, this, [this] () { Q_EMIT nextClicked(); });

    ui->prevButton->setIcon(QIcon(":/icons/resources/prev-black.svg"));
    ui->nextButton->setIcon(QIcon(":/icons/resources/next-black.svg"));
    ui->pausePlayButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
}

DDEMpris2Widget::~DDEMpris2Widget()
{
    delete ui;
}

void DDEMpris2Widget::showStatus(PlayerStatus status) {
    this->currStatus = status;

    ui->titleLabel->setText(this->currStatus.getTitle());
    ui->artistLabel->setText(this->currStatus.getArtist());
    ui->albumLabel->setText(this->currStatus.getAlbum());

    QUrl url = this->currStatus.getArtUrl();
    if (url.isValid()) {
        QImage image(url.path());
        ui->artLabel->setPixmap(QPixmap::fromImage(image).scaled(ui->artLabel->size()));
    }
    // the position in metadata is not reliable for some applications
    //    we use dbus + timer for position instead
    //    this->updatePosition(status.getPosition());

    ui->nextButton->setEnabled(status.getCanGoNext());
    ui->prevButton->setEnabled(status.getCanGoPrevious());
    ui->pausePlayButton->setEnabled(status.getCanPlay() && status.getCanPause());
}

void DDEMpris2Widget::updatePosition(qlonglong position) {
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
        ui->pausePlayButton->setIcon(QIcon(":/icons/resources/play-black.svg"));
    } else {
        ui->pausePlayButton->setIcon(QIcon(":/icons/resources/pause-black.svg"));
    }
}

