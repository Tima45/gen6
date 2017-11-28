#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QReadWriteLock>
#include <QDebug>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = 0);
    bool isPlaying = false;
    void tern();
signals:

public slots:
    void infinitGamePlaying();
};

#endif // GAME_H
