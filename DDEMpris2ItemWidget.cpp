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
