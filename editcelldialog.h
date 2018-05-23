#ifndef EDITCELLDIALOG_H
#define EDITCELLDIALOG_H

#include <QDialog>
#include "game.h"
#include "bot.h"
#include "empty.h"
#include "genomcommandlistwidgetitem.h"

namespace Ui {
class EditCellDialog;
}

class EditCellDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCellDialog(QWidget *parent = 0);
    ~EditCellDialog();
    void setCell(Cell* cell);
    Cell* editingCell = nullptr;
signals:
    void redrawWorld();

private slots:
    void on_cellTypeBox_currentIndexChanged(int index);

    void on_genomCommandsList_itemDoubleClicked(QListWidgetItem *item);

    void on_buttonBox_accepted();

    void on_genomList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::EditCellDialog *ui;
};

#endif // EDITCELLDIALOG_H
