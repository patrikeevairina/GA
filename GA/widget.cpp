#include "widget.h"
#include "ui_widget.h"

static int NOD(int a, int b){
    while (a != b){
        if (a > b){
            a -= b;
        } else {
            b -= a;
        }
    }
    return a;
}

static int NOK(int a, int b){
    return abs(a * b)/NOD(a, b);
}


Widget::Widget(int N, QWidget *parent)
    : QWidget(parent), N(N)
{
    image = new QImage(":/image/orange.jpg");
    size = image->size()/image->devicePixelRatio();
    // qDebug() << size;
    targetFunc = new QList<QColor>;
    currentFunc = new QList<QColor>;

    file = new QTemporaryFile();

    this->setFocusPolicy(Qt::StrongFocus);
    scene = new QGraphicsScene();
    scene->setSceneRect(image->rect());


    // scene->addPixmap(QPixmap::fromImage(*image)); ВАЖНО

    view = new QGraphicsView(scene);
    // view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->resize(size.width(), size.height());
    view->setFixedSize(size.width(), size.height());
    view->setMouseTracking(true);
    view->show();

    targetFunction();
    randomPolygons();
    drawPolygons();

    QWidget *menu = new QWidget();
    menu->setGeometry(20, 20, 170, 140);
    menu->setStyleSheet("QWidget {background-color: rgb(240,240,240);}");
    menu->show();

    auto *button_1 = new QPushButton(QString("Размножить"), menu);
    connect(button_1, &QPushButton::clicked, [=]()
    {

        qDebug() << polygons.size();
        crossingover(true);
        drawPolygons();

    });
    button_1->setGeometry(10, 10, 150, 20);
    button_1->show();

    auto *button_2 = new QPushButton(QString("Exit"), menu);
    connect(button_2, &QPushButton::clicked, [=]()
    {
        exit(0);
    });
    button_2->setGeometry(10, 60, 150, 20);
    button_2->show();

    auto *button_3 = new QPushButton(QString("Selection"), menu);
    connect(button_3, &QPushButton::clicked, [=]()
    {
        selection();

    });
    button_3->setGeometry(10, 35, 150, 20);
    button_3->show();
    auto *button_4 = new QPushButton(QString("Draw polygons"), menu);
    connect(button_4, &QPushButton::clicked, [=]()
    {
        drawPolygons();

    });
    button_4->setGeometry(10, 85, 150, 20);
    button_4->show();
    auto *button_5 = new QPushButton(QString("Save in buf.png"), menu);
    connect(button_5, &QPushButton::clicked, [=]()
    {
        QPixmap pixMap = view->grab(view->rect());
        pixMap.save("buf.png");

    });
    button_5->setGeometry(10, 110, 150, 20);
    button_5->show();
}

void Widget::randomPolygons()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution(1, 11520);
    qDebug() << distribution(generator);

    for (int i = 0; i < N; ++i)
    {
        polygons.push_back(new Polygon(distribution(generator)%64, distribution(generator)%64, distribution(generator)%360, distribution(generator)%256,
                                       distribution(generator)%256, distribution(generator)%256, 255));
    }
}



Polygon* Widget::crossPolygons(Polygon &mother, Polygon &father)
{   
    qreal width = (mother.rect.width() + father.rect.width())/2;
    qreal height = (mother.rect.height() + father.rect.height())/2;
    qreal rotation = (mother.rotation + father.rotation)/2;

    int r = (mother.color.red() + father.color.red())/2;
    int g = (mother.color.green() + father.color.green())/2;
    int b = (mother.color.blue() + father.color.blue())/2;
    int a = (mother.color.alpha() + father.color.alpha())/2;

    Polygon *p = new Polygon(width, height, rotation, r, g, b, a);
    return  p;
}

void Widget::crossingover(bool mutationMode)
{
    int magicNumber = NOK(size.width(), size.height());
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution(1, magicNumber);
    int k = polygons.size();
    int check = 0;
        for (int i = 0; i < k/2; ++i)
        {
            if (distribution(generator) < (qreal)(crossPossibility*magicNumber))
            {
                if (distribution(generator) <  (qreal)(mutationPossibility*magicNumber) && mutationMode)
                {
                    polygons.push_back(new Polygon(distribution(generator)%64, distribution(generator)%64, distribution(generator)%360, distribution(generator)%256,
                                                   distribution(generator)%256, distribution(generator)%256, 255));
                    check++;
                }
                else
                {
                    polygons.push_back(crossPolygons(*polygons.at(i), *polygons.at(k-i-1)));
                }
            }
        }
}

void Widget::drawPolygons()
{
    //    QPixmap pixMap = view->grab(view->rect());
    //    pixMap.save(file);
    //    scene->addPixmap(pixMap);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution(1, size.width()*size.height());
    //    for (int i = 0; i < polygons.size(); ++i)
    //    {
    //        if (polygons.at(i) -> isDrawed == true)
    //        {
    //            polygons.at(i)->deleteLater();
    //            polygons.erase(polygons.begin()+i);
    //        }
    //    }
    int k = 0;
    for (auto i: polygons)
    {
        if (i->isDrawed == false)
        {
            ++k;
            scene->addItem(i);
            i->_x_y  = QPointF(distribution(generator)%size.width(), distribution(generator)%size.height());
            i->setPos(i->_x_y);
            //i->show();
            i->isDrawed = true;
        }
    }

    currentFunction();
    //qDebug() << "debug " << k;
}

void Widget::targetFunction()
{
    for (int y = 0; y < image->height(); ++y)
    {
        for (int x = 0; x < image->width(); ++x)
        {
            //qDebug() << image->pixelColor(x,y).rgba();
            targetFunc->push_back(image->pixelColor(x,y));
        }
    }

}

void Widget::currentFunction()
{
    QPixmap pixMap = view->grab(view->rect());

    currentFunc->clear();
    for (int y = 0; y < image->height(); ++y)
    {
        for (int x = 0; x < image->width(); ++x)
        {
            currentFunc->push_back(pixMap.toImage().pixelColor(x,y));
        }
    }
}



void Widget::selection()
{
    currentFunction(); //потом убрать

    int devByRed = 0;
    int devByGreen = 0;
    int devByBlue = 0;
    long resDev = 0;
    for (int i = 0; i < currentFunc->size(); ++i)
    {
        devByRed = pow((targetFunc->at(i).red() - currentFunc->at(i).red()), 2);
        devByGreen = pow((targetFunc->at(i).green() - currentFunc->at(i).green()), 2);
        devByBlue = pow((targetFunc->at(i).blue() - currentFunc->at(i).blue()), 2);
        resDev = pow((devByRed+devByBlue+devByGreen), 0.5);
        if (resDev > 64 && scene->itemAt(QPointF(i/image->width(), i % image->width()), QTransform()))
        {
            static_cast<Polygon*>(scene->itemAt(QPointF(i/image->width(), i % image->width()), QTransform()))->isKilled = true;
        }
    }

    for (int i = 0; i < polygons.size(); ++i)
    {
        if (polygons.at(i)->isKilled)
        {
            polygons.at(i)->deleteLater();
            polygons.erase(polygons.begin()+i);
        }
        else
        {
            //polygons.at(i)->isStrong = true;
        }
    }
    drawPolygons();

    QPixmap pixMap = view->grab(view->rect());
    pixMap.save(file);
    scene->addPixmap(pixMap);
    for (int i = 0; i < polygons.size(); ++i)
    {
        if (polygons.at(i) -> isDrawed == true)
        {
            polygons.at(i)->deleteLater();
            polygons.erase(polygons.begin()+i);
        }
    }
    drawPolygons();
}


Widget::~Widget()
{

}

