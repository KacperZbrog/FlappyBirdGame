#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <Qtimer>
#include "pillaritem.h"
#include "birditem.h"

#include <QObject>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();
    void startGame();

    bool getGameOn() const;
    void setGameOn(bool value);

    void incrementScore();

    void setScore(int value);

signals:
public slots:
    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    void showSign();
    void hideSign();
    void showGameOverGraphics();
    void hideGameOverGraphics();
    void cleanPillars();
    void setUpPillarTimer();
    void freezeBirdAndPillarsInPlace();

    QTimer * pillarTimer;
    BirdItem * bird;
    bool gameOn;

    int score;
    int bestScore;

    QGraphicsPixmapItem * gameSignPix;
    QGraphicsPixmapItem * gameOverPix;
    QGraphicsTextItem * scoreTextItem;
};

#endif // SCENE_H
