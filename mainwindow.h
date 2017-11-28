#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QReadWriteLock>
#include "plot/qcustomplot.h"
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QThread *gameThread;
    Game *game;
    void initPlot();
signals:
    void startGame();


private slots:
    void on_newWorldButton_clicked();

    void on_startStopButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
