#ifndef HWDIALOG_H
#define HWDIALOG_H

#include <QDialog>
#include "vehiclevalues.h"

namespace Ui {
class HWDialog;
}

class HWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HWDialog(VehicleValues &vehicle, QWidget *parent = 0);
    ~HWDialog();

public slots:
    void updateValues();
    void accept();
    void reject();

private slots:

private:
    Ui::HWDialog *ui;
    VehicleValues *vehicle;

    void showEvent(QShowEvent *event);
};

#endif // HWDIALOG_H
