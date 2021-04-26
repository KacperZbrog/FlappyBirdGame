#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

Scene::Scene(QObject *parent) : QGraphicsScene(parent),
    gameOn(false),score(0),bestScore(0)
{
    setUpPillarTimer();
}

void Scene::addBird()
{
    bird = new BirdItem(QPixmap(":/images/bird_up.png"));
    addItem(bird);
    showSign();
}

void Scene::startGame()
{
    bird->startFlying();

    if(!pillarTimer->isActive())
    {
        cleanPillars();
        hideSign();
        setGameOn(true);
        setScore(0);
        hideGameOverGraphics();
        pillarTimer->start(1000);
    }
}

void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer,&QTimer::timeout,[=]
    {
        PillarItem * pillarItem = new PillarItem();
        connect(pillarItem,&PillarItem::colliedFail,[=]()
        {
            pillarTimer->stop();
            freezeBirdAndPillarsInPlace();
            setGameOn(false);
            showGameOverGraphics();
        });
        addItem(pillarItem);
    });
    //pillarTimer->start(1000);
}

void Scene::freezeBirdAndPillarsInPlace()
{
    bird->freezeInPlace();

    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item, sceneItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem* >(item);
        if(pillar)
        {
            pillar->freezeInPlace();
        }
    }
}

void Scene::setScore(int value)
{
    score = value;
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool value)
{
    gameOn = value;
}

void Scene::incrementScore()
{
    score++;
    if(score > bestScore)
    {
        bestScore = score;
        qDebug() << "Score : " << score <<"Best Score : " << bestScore;
    }
}
void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        if(gameOn)
        {
           bird->shootUp();
        }
    }
    //QGraphicsScene:keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(gameOn)
        {
            bird->shootUp();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::showSign()
{
    gameSignPix = new QGraphicsPixmapItem(QPixmap(":/images/sign.png"));
    addItem(gameSignPix);
    gameSignPix->setPos(QPointF(0,0) - QPointF(gameSignPix->boundingRect().width()/2,gameSignPix->boundingRect().height()/0.5));
}

void Scene::hideSign()
{
    if(gameSignPix)
    {
        removeItem(gameSignPix);
        delete gameSignPix;
        gameSignPix = nullptr;
    }
}

void Scene::showGameOverGraphics()
{
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/game_over.png"));
    addItem(gameOverPix);
    gameOverPix->setPos(QPointF(0,0) - QPointF(gameOverPix->boundingRect().width()/2.3,gameOverPix->boundingRect().height()/4));

    scoreTextItem = new QGraphicsTextItem();
    QString htmlString = "<p> Score: " + QString::number(score) + " </p>" + "<p> Best Score: " + QString::number(bestScore) + " </p>";

    QFont mFont("Consolas",30,QFont::Bold);

    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(mFont);
    scoreTextItem->setDefaultTextColor(Qt::red);
    addItem(scoreTextItem);

    scoreTextItem->setPos(QPointF(0,0) - QPointF(scoreTextItem->boundingRect().width()/2,gameOverPix->boundingRect().height()/2));
}

void Scene::hideGameOverGraphics()
{
    if(gameOverPix)
    {
        removeItem(gameOverPix);
        delete gameOverPix;
        gameOverPix = nullptr;
    }
    if(scoreTextItem)
    {
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

void Scene::cleanPillars()
{
    QList<QGraphicsItem*> sceneItems = items();
    foreach(QGraphicsItem *item,sceneItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem*>(item);
        if(pillar)
        {
            removeItem(pillar);
            delete pillar;
        }
    }
}
