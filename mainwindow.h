#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <QReadWriteLock>
#include "plot/qcustomplot.h"
#include "game.h"
#include "doublecolors.h"
#include <QTimer>

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

    Cell *trackingCell;
    bool tracking = false;

    QReadWriteLock locker;

    QCPColorMap *colorMap;
    QCPColorGradient commandsGradient;
    QCPColorGradient energyGradient;
    QCPColorGradient genomdiffGradient;

    QCPItemRect *cursor;
    QCPItemTracer *traceLeftTop;
    QCPItemTracer *traceRightBottom;

    QCPGraph *aliveGraph;
    QCPGraph *deadGraph;

    QTimer *fpsTimer;
    uint lastFps = 0;

    void initPlot();
signals:
    void startGame();
    void playOneTern();



private slots:
    void on_newWorldButton_clicked();

    void on_startStopButton_clicked();

    void updateLabels(uint tern, uint alive, uint dead);


    void on_skipReplotCheck_clicked(bool checked);

    void fpsCount();
    void rangeLimitation(QCPRange);
    void handleMouseClicking(QMouseEvent *event);

    void on_comboBox_currentIndexChanged(int index);

    void on_rescaleButton_clicked();

    void on_nextTernButton_clicked();
    void displayBotInfo(Bot*bot);

private:
    Ui::MainWindow *ui;
    int lastTern = 0;
};

#endif // MAINWINDOW_H
