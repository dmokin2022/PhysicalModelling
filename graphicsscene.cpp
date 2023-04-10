#include "graphicsscene.h"

GraphicsScene::GraphicsScene()
{

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    addEllipse(event->scenePos().x()-5,
               event->scenePos().y()-5,
               10,
               10,
               QPen(Qt::NoPen),
               QBrush(Qt::red));
    emit clicked_(event->scenePos().x(), event->scenePos().y());

}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    qreal x = event->scenePos().x();
    qreal y = event->scenePos().y();

    addLine(prevX, prevY, x, y);

    prevX = x;
    prevY = y;

    emit moved_(x, y);
}


void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}
