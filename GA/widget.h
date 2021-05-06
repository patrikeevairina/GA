#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <random>
#include <chrono>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QImage>
#include <QLabel>
#include <cmath>
#include <QRgb>
#include <QList>
#include <QTransform>
#include <QFileDialog>
#include <QTimer>
#include <QSvgGenerator>
#include <QFileDialog>
#include <QPainter>
#include <numeric>
#include <QDir>
#include <QTemporaryFile>


#include "polygon.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(int N, QWidget *parent = nullptr);
    ~Widget();
    void randomPolygons();
    Polygon* crossPolygons(Polygon &mother, Polygon &father);
    void crossingover(bool mutationMode);
    void drawPolygons();
    void selection();

    void targetFunction();
    void currentFunction();

    QTemporaryFile *file;



private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QImage *image;
    QSize size;
    QString path;

//    int _width = 1280;
//    int _height = 785;
    QVector<Polygon*> polygons;
    QList<QColor> *targetFunc;
    QList<QColor> *currentFunc;

    bool mutationMode = true;
    qreal crossPossibility = 0.7;//вероятность кроссинговера
    qreal mutationPossibility = 0.1;
    int N;//N- count of first generation

};
#endif // WIDGET_H
