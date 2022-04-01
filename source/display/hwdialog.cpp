#include <QTimer>
#include <QMessageBox>
#include <QDebug>
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
    if (isVisible() && vehicle->initialized) {
        ui->plainTextEdit->setPlainText(vehicle->serialize());
        ui->ppmCurrent->setText(QString::number(vehicle->getPPM()));
        if (ui->ppmSpinBox->value() != vehicle->getPPM()) {
            QPalette pal = ui->ppmSpinBox->palette();
            pal.setColor(ui->ppmSpinBox->backgroundRole(), Qt::red);
            ui->ppmSpinBox->setPalette(pal);
        } else {
            QPalette pal = ui->ppmSpinBox->palette();
            pal.setColor(ui->ppmSpinBox->backgroundRole(), Qt::gray);
            ui->ppmSpinBox->setPalette(pal);
        }
    }

    update();
}

void HWDialog::showEvent(QShowEvent *event) {
    ui->ppmSpinBox->setValue(vehicle->getPPM());
    ui->ppmSpinBox->setStyleSheet("QSpinBox { background-color: white; }");
}

void HWDialog::accept()
{
    vehicle->newPPM = ui->ppmSpinBox->value();
    vehicle->writePPM = true;

    QMessageBox msgBox;
    msgBox.setText("Values have been saved to EEPROM.");
    msgBox.exec();
}

void HWDialog::reject()
{
    this->done(0);
}
