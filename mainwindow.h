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
#include <QListWidgetItem>
#include "worldparametersdialog.h"
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include "genomcommandlistwidgetitem.h"
#include "editcelldialog.h"


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

    Cell *trackingCell = nullptr;
    bool tracking = false;

    QReadWriteLock locker;

    QCPColorMap *colorMap;
    QCPColorGradient styleGradient;
    QCPColorGradient energyGradient;
    QCPColorGradient killGradient;
    QCPColorGradient cloneGradient;
    QCPColorGradient defenceGradient;
    QCPColorGradient longLiveGradient;
    QCPColorGradient amountGradient;


    QCPItemRect *cursor;
    QCPItemTracer *traceLeftTop;
    QCPItemTracer *traceRightBottom;
    QCPItemLine *dirArrow;

    QCPGraph *aliveGraph;
    QCPGraph *deadGraph;
    QCPItemTracer *aliveTracer;
    QCPItemTracer *deadTracer;

    QSharedPointer<QCPAxisTickerFixed> fixedTicker;

    QTimer *fpsTimer;
    uint lastFps = 0;

    void initPlot();
    void initListWidget();
signals:
    void startGame();

    void playOneTurn();



private slots:
    void stopTrackingCell();
    void resetColorMapRange(int size);

    void resetWorldRange(int size);

    void redrawWorld();

    void on_newWorldButton_clicked();

    void on_startStopButton_clicked();

    void updateLabels(uint turn, uint alive, uint dead);


    void on_skipReplotCheck_clicked(bool checked);

    void fpsCount();
    void rangeLimitation(QCPRange);
    void handleMouseClicking(QMouseEvent *event);

    void on_comboBox_currentIndexChanged(int index);

    void on_rescaleButton_clicked();

    void on_nextTurnButton_clicked();
    void displayBotInfo(Bot*bot);

    void on_worldParameternsButton_clicked();

    void on_saveWorldButton_clicked();

    void on_loadWorldButton_clicked();

    void on_clearGraphButton_clicked();

    void on_genomList_itemDoubleClicked(QListWidgetItem *item);

    void on_editButton_clicked();

    void on_editButton_2_clicked();


    void on_clearSpecialColorButton_clicked();

private:
    Ui::MainWindow *ui;
    int lastTurn = 0;
    WorldParametersDialog *worldParametersDialog;
    EditCellDialog *editCellDialog;
    void stopGame();
    void editCell();
};

#endif // MAINWINDOW_H
