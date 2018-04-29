#ifndef WORLDPARAMETERSDIALOG_H
#define WORLDPARAMETERSDIALOG_H

#include <QDialog>
#include "bot.h"
#include "empty.h"

namespace Ui {
class WorldParametersDialog;
}

class WorldParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorldParametersDialog(QWidget *parent = 0);
    ~WorldParametersDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::WorldParametersDialog *ui;
};

#endif // WORLDPARAMETERSDIALOG_H
