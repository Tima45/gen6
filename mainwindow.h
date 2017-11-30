#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QReadWriteLock>
#include <QTimer>
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
    QReadWriteLock locker;

    QCPColorMap *colorMap;
    QCPColorGradient commandsGradient;

    void initPlot();
    QTimer *timer;
signals:
    void startGame();


private slots:
    void on_newWorldButton_clicked();

    void on_startStopButton_clicked();

    void updateLabels(uint tern,uint ternDuration,uint alive,uint dead);


    void on_skipReplotCheck_clicked(bool checked);
    void fps();

private:
    Ui::MainWindow *ui;
    int lastTern = 0;
};

#endif // MAINWINDOW_H
