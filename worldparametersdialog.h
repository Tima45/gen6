#ifndef WORLDPARAMETERSDIALOG_H
#define WORLDPARAMETERSDIALOG_H

#include <QDialog>
#include "plot/qcustomplot.h"
#include "bot.h"
#include "empty.h"
#include "game.h"

namespace Ui {
class WorldParametersDialog;
}

class WorldParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorldParametersDialog(QWidget *parent = 0);
    ~WorldParametersDialog();
    void loadParametres();
signals:
    void changeWorldSize(int size);

private slots:
    void on_buttonBox_accepted();

    void on_rottingDeadBox_toggled(bool checked);

    void on_startBotEnergyBox_valueChanged(double arg1);

private:
    Ui::WorldParametersDialog *ui;
};

#endif // WORLDPARAMETERSDIALOG_H
