#ifndef POLYGON_H
#define POLYGON_H

#include <QObject>
#include <QPolygon>
#include <QPainter>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QColor>
#include <QDebug>
#include <QGraphicsObject>
#include <chrono>
#include <random>

class Polygon: public QGraphicsObject
{
    Q_OBJECT
public:
    Polygon(qreal width, qreal height, qreal rotation, int r, int g, int b, int a = 255, QObject *parent = nullptr);
    ~Polygon(){}
    QPainterPath shape() const override;

    bool isDrawed = false;
    bool isKilled = false;
    bool isStrong = false;

    QPointF _x_y;
    qreal rotation;
    QRectF rect;
    QColor color;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // POLYGON_H
