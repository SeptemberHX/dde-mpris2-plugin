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
    this->updatePosition(status.getPosition());
}

void DDEMpris2Widget::updatePosition(qlonglong position) {
    qlonglong leftTime = this->currStatus.getLength() - position;
    ui->leftLabel->setText(QString("-%1:%2").arg(leftTime / 1000000 / 60).arg(leftTime / 1000000 % 60, 2, 10, QLatin1Char('0')));
    ui->posLabel->setText(QString("%1:%2").arg(
            position / 1000000 / 60).arg(position / 1000000 % 60, 2, 10, QLatin1Char('0')));
    ui->progressBar->setValue((int) (position * 1.0 / this->currStatus.getLength() * 100));
}
