#include "pillaritem.h"
#include "birditem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <scene.h>

PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pilar.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pilar.png"))),
    pastBird(false)
{
    topPillar->setPos(QPoint(0,0) - QPointF(topPillar->boundingRect().width()/2,topPillar->boundingRect().height() +100));
    bottomPillar->setPos(QPointF(0,0) + QPointF(-bottomPillar->boundingRect().width()/2,60));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

     yPos = QRandomGenerator::global()->bounded(150);
     xRandomizer = QRandomGenerator::global()->bounded(200);

     setPos(QPoint(0,0) + QPoint(260 + xRandomizer,yPos));

    xAnimation = new QPropertyAnimation(this,"x",this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-400);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation,&QPropertyAnimation::finished,[=]()
    {
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();


}

PillarItem::~PillarItem()
{
    delete topPillar;
    delete bottomPillar;
}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::freezeInPlace()
{
    xAnimation -> stop();
}

void PillarItem::setX(qreal x)
{
    m_x = x;

    if(x < 0 && !pastBird)
    {
        pastBird = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mScene);
        if(myScene)
        {
            myScene->incrementScore();
        }


    }

    if(collidesWithBird())
    {
        emit colliedFail();
    }
    setPos(QPointF(0,0) + QPointF(x,yPos));
}

bool PillarItem::collidesWithBird()
{
    QList<QGraphicsItem*> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());
    foreach(QGraphicsItem * item, collidingItems)
    {
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem)
        {
            return true;
        }
    }
    return false;
}
