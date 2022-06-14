#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include "hwdialog.h"
#include "ui_hwdialog.h"

HWDialog::HWDialog(VehicleValues &vehicle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HWDialog)
{
    ui->setupUi(this);

    this->vehicle = &vehicle;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HWDialog::updateValues);
    timer->start(100); // ~10 FPS
}

HWDialog::~HWDialog()
{
    delete ui;
}

void HWDialog::updateValues()
{
    if (isVisible() && vehicle->getSerialConnected()) {
        ui->plainTextEdit->setPlainText(vehicle->serialize());
        ui->configEdit->setPlainText(vehicle->config.serialize());

        ui->ppmCurrent->setText(QString::number(vehicle->config.getPPM()));

        if (ui->ppmSpinBox->value() != vehicle->config.getPPM()) {
            QPalette pal = ui->ppmSpinBox->palette();
            pal.setColor(ui->ppmSpinBox->backgroundRole(), Qt::red);
            ui->ppmSpinBox->setPalette(pal);
        } else {
            QPalette pal = ui->ppmSpinBox->palette();
            pal.setColor(ui->ppmSpinBox->backgroundRole(), Qt::gray);
            ui->ppmSpinBox->setPalette(pal);
        }

        if (ui->screenDimmingSpinBox->value() != vehicle->config.getScreenDimming()) {
            QPalette pal = ui->screenDimmingSpinBox->palette();
            pal.setColor(ui->screenDimmingSpinBox->backgroundRole(), Qt::red);
            ui->screenDimmingSpinBox->setPalette(pal);
        } else {
            QPalette pal = ui->screenDimmingSpinBox->palette();
            pal.setColor(ui->screenDimmingSpinBox->backgroundRole(), Qt::gray);
            ui->screenDimmingSpinBox->setPalette(pal);
        }
    }

    update();
}

void HWDialog::showEvent(QShowEvent *event) {
    ui->ppmSpinBox->setValue(vehicle->config.getPPM());
    ui->ppmSpinBox->setStyleSheet("QSpinBox { background-color: white; }");

    ui->blinkerBox->setChecked(this->vehicle->config.getBlinkerSound());
    ui->blinkerBox->setStyleSheet("QSpinBox { background-color: white; }");

    ui->screenDimmingSpinBox->setValue(this->vehicle->config.getScreenDimming());
    ui->screenDimmingSpinBox->setStyleSheet("QSpinBox { background-color: white; }");
}

void HWDialog::accept()
{
    QString toWrite = QString("wconfig:%1,%2,%3")
            .arg(ui->ppmSpinBox->value())
            .arg(ui->blinkerBox->isChecked())
            .arg(ui->screenDimmingSpinBox->value());

    QMutexLocker locker(&vehicle->queueMutex);
    vehicle->serialWriteQueue.enqueue(toWrite);
    this->done(1);
}

void HWDialog::reject()
{
    this->done(0);
}
