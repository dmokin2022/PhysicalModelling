#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene();
signals:
    void clicked_(qreal x, qreal y);
    void moved_(qreal x, qreal y);


private:
    qreal prevX;
    qreal prevY;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // GRAPHICSSCENE_H
