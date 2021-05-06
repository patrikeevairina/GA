#include "polygon.h"

Polygon::Polygon(qreal width, qreal height, qreal rotation, int r, int g, int b, int a, QObject *parent)
    :rotation(rotation), rect(0,0,height,width), color(r,g,b,a)
{
    Q_UNUSED(parent);
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->rotate(rotation);
    painter->setPen(color);
    painter->setBrush(QBrush(color));
    painter->drawEllipse(rect);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Polygon::boundingRect() const
{
    return rect;
}

QPainterPath Polygon::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

