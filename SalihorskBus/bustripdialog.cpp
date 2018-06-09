#include "bustripdialog.h"
#include "ui_bustripdialog.h"

void BusTripDialog::stationClicked(QListWidgetItem* station) {
    this->done(station->listWidget()->row(station) + 1);
}

BusTripDialog::BusTripDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::BusTripDialog) {
    ui->setupUi(this);
    connect(ui->tripList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(stationClicked(QListWidgetItem*)));

}

BusTripDialog::~BusTripDialog() {
    delete ui;
}

void BusTripDialog::addStation(QString stationName, QString time) {
    QListWidgetItem* station = new QListWidgetItem(stationName + "\n" + time, ui->tripList);
    QSize itemSize;
    itemSize.setHeight(40);
    station->setSizeHint(itemSize);
    station->setTextAlignment(Qt::AlignHCenter);

}
