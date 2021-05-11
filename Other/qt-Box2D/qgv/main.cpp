#include <Box2D.h>
#include <QtGui>
#include <QApplication>
#include<QGraphicsView>
#include<QGraphicsScene>
#include <QGraphicsPolygonItem>

static const int BODYCOUNT = 100;
static const int32 B2_ITERATIONS = 10;
static const float32 B2_TIMESTEP = 1.0f / 30.0f;
static const float PI = 3.14159265359;

class QGraphicsBox2DPolygonItem : public QGraphicsPolygonItem
{
public:
    QGraphicsBox2DPolygonItem(b2World *world, QGraphicsItem *parent = 0)
        : QGraphicsPolygonItem(parent), _world(world), _body(0)
    { }

    void setup()
    {
        // Create body with start-up position and rotation.
        b2BodyDef bodyDef;
        bodyDef.position.Set(x(), -y());
        bodyDef.angle = -(rotation() * (2 * PI)) / 360.0;
        _body = _world->CreateBody(&bodyDef);

        // Assign shape to polygon body.
        b2PolygonShape myShap;

        b2FixtureDef fixtureDef;
        fixtureDef.shape=&myShap;
        fixtureDef.density=1.0f;
        fixtureDef.friction=0.5f;


        const QPolygonF &poly = polygon();
        int n = poly.size();
        //create a vertic array
         b2Vec2* vertices=new b2Vec2[poly.size()]();

        for (int i = 0; i < n; ++i) {
            const QPointF &p = poly.at(i);
           vertices[i].Set(p.x(), -p.y());
        }

         _body->CreateFixture(&fixtureDef);

        myShap.Set(vertices,poly.size());

        // Setup default mass.

//        _body->SetMassFromShapes();
    }

    void adjust()
    {
        // Update QGraphicsItem's position and rotation from body.
        b2Vec2 position = _body->GetPosition();
        float32 angle = _body->GetAngle();
        setPos(position.x, -position.y);
        setRotation(-(angle * 360.0) / (2 * PI));
    }

private:
    b2World *_world;
    b2Body *_body;
};

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(QGraphicsScene *scene, QObject *parent = 0)
        : QObject(parent), timerId(0), scene(scene)
    {
        // Define world, gravity
        b2AABB worldAABB;
        worldAABB.lowerBound.Set(-200, -100);
        worldAABB.upperBound.Set(200, 500);

        world = new b2World(/*worldAABB,*/
                            /* gravity = */ b2Vec2(0.0f, -10.0f)
                            );//,/* doSleep = */ true


        // Background item (not part of b2 world)
        QGraphicsRectItem *backBlock = scene->addRect(-100, -40, 200, 60);
        backBlock->setBrush(QColor(93, 83, 37));
        backBlock->setZValue(-1);
        backBlock->setPen(QPen(Qt::black, 1));

        b2BodyDef bodyDef;
        b2PolygonShape myShape2;
        b2FixtureDef fixture2;
        fixture2.shape=&myShape2;


//        b2PolygonDef shapeDef;

        // Rigid ground item
        QGraphicsRectItem *groundItem = scene->addRect(-100, -10, 200, 20);
        groundItem->setParentItem(backBlock);
        groundItem->setBrush(QColor(178, 192, 160));
        groundItem->setPos(0, 10);
        groundItem->setPen(Qt::NoPen);
        bodyDef.position.Set(0.0f, -10);
//        shapeDef.SetAsBox(100.0f, 10.0f);
        myShape2.SetAsBox(100.0f, 10.0f);
        groundBody = world->CreateBody(&bodyDef);
//        groundBody->CreateShape(&shapeDef);
        groundBody->CreateFixture(&fixture2);

        // Rigid left block item
        QGraphicsRectItem *leftBlockItem = scene->addRect(-5, -15, 10, 45);
        leftBlockItem->setParentItem(backBlock);
        leftBlockItem->setPen(Qt::NoPen);
        leftBlockItem->setBrush(QColor(178, 192, 160));
        leftBlockItem->setPos(-95, -25);
        bodyDef.position.Set(-95.0f, 20.0f);

        b2PolygonShape myshap3;


//        shapeDef.SetAsBox(5.0f, 20.0f);
        myshap3.SetAsBox(5.0f, 20.0f);
        b2FixtureDef fixture3;
        fixture3.shape=&myshap3;

        b2Body *leftBody = world->CreateBody(&bodyDef);
        leftBody->CreateFixture(&fixture3);

        // Rigid right block item
        QGraphicsRectItem *rightBlockItem = scene->addRect(-5, -15, 10, 45);
        rightBlockItem->setParentItem(backBlock);
        rightBlockItem->setPen(Qt::NoPen);
        rightBlockItem->setBrush(QColor(178, 192, 160));
        rightBlockItem->setPos(95, -25);
        bodyDef.position.Set(95.0f, 20.0f);
//        shapeDef.SetAsBox(5.0f, 20.0f);
        b2Body *rightBody = world->CreateBody(&bodyDef);
        rightBody->CreateFixture(&fixture3);

        // Rigid elllipse item
//        QGraphicsEllipseItem *ellipse = scene->addEllipse(-5, -5, 10, 10);
//        ellipse->setPos(0, -55);
//        ellipse->setPen(Qt::NoPen);
//        ellipse->setBrush(Qt::black);
//        bodyDef.position.Set(0.0f, 55.0f);
//        b2CircleDef ellipseDef;
//        ellipseDef.radius = 5;
//        b2Body *ellipseBody = world->CreateBody(&bodyDef);
//        ellipseBody->CreateShape(&ellipseDef);

        // Rigid elllipse item2
//      /*  QGraphicsEllipseItem *ellipse2 = scene->addEllipse(-5, -5, 10, 10);
//        ellipse2->setPos(-30, -50);
//        ellipse2->setPen(Qt::NoPen);
//        ellipse2->setBrush(Qt::black);
//        bodyDef.position.Set(-30.0f, 50.0f);
//        b2Body *ellipseBody2 = world->CreateBody(&bodyDef);
//        e*/llipseBody2->CreateShape(&ellipseDef);

        // Rigid elllipse item2
//        QGraphicsEllipseItem *ellipse3 = scene->addEllipse(-5, -5, 10, 10);
//        ellipse3->setPos(30, -50);
//        ellipse3->setPen(Qt::NoPen);
//        ellipse3->setBrush(Qt::black);
//        bodyDef.position.Set(30.0f, 50.0f);
//        b2Body *ellipseBody3 = world->CreateBody(&bodyDef);
//        ellipseBody3->CreateShape(&ellipseDef);

        // Create lots of little colored triangles, random pos, rotation, color.
        QPolygonF poly;
        poly << QPointF(0, -10) << QPointF(-5, 0) << QPointF(5, 0);
        for (int i = 0; i < BODYCOUNT; ++i) {
            QGraphicsBox2DPolygonItem *polygon = bodyItems[i] = new QGraphicsBox2DPolygonItem(world);
            polygon->setPos(-20 + qrand() % 40, -75 - qrand() % 150);
            polygon->setRotation(qrand() % 360);
            polygon->setPolygon(poly);
            polygon->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            polygon->setup();
            scene->addItem(polygon);
        }
    }

    void start()
    {
        if (!timerId)
            timerId = startTimer(1000 / 60.0);
    }

    ~Simulator()
    {
        delete world;
    }

protected:
    void timerEvent(QTimerEvent *event)
    {
        if (event->timerId() == timerId) {
            world->Step(B2_TIMESTEP,1,B2_ITERATIONS);
            for (int i = 0; i < BODYCOUNT; ++i)
                bodyItems[i]->adjust();
        }
        QObject::timerEvent(event);
    }

private:
    int timerId;
    b2World *world;
    b2Body *groundBody;
    QGraphicsScene *scene;
    QGraphicsBox2DPolygonItem *bodyItems[BODYCOUNT];
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qsrand(time(0));

    QGraphicsScene scene;
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    scene.setBackgroundBrush(Qt::white);
    scene.setSceneRect(-110, -150, 220, 175);

    QGraphicsView view(&scene);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setRenderHint(QPainter::Antialiasing);
    view.scale(4, 4);
    view.show();

    Simulator simulator(&scene);
    simulator.start();

    return app.exec();
}

#include "main.moc"
