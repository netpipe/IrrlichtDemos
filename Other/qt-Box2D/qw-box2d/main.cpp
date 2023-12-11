#include <QtGui>
#include <QtOpenGL>
#include <Box2D/Box2D.h>

// 360x640 px = 36x64m

enum {
    BallObject,
    WallObject,
} Objects;

struct Object
{
    int type;
    b2Body *body;
    b2Fixture *fixture;
};

class World : public QWidget {
public:
    World()
        : _timerId(0)
    {
        b2Vec2 gravity(0.0f, -10.0f);
        bool doSleep(true);
        _world = new b2World(gravity);

        for(int i=0; i<30; i++) {
            int dx = 18 - qrand() % 36;
            int dy = qrand() % 2;
            _objects.append(createBall(b2Vec2(18.0f+dx, 62.0f-dy), 1.0f));
        }

        _transform.scale(10.0f, -10.0f);
        _transform.translate(0.0f, -64.0f);
        qDebug() << _transform.map(QPointF(0.0f,0.0f));
        qDebug() << _transform.map(QPointF(36.0f,64.0f));
        _objects.append(createWall(0.0f, 0.0f, 36.0f, 1.0f));
        _objects.append(createWall(0.0f, 0.0f, 1.0f, 64.0f));
        _objects.append(createWall(35.0f, 0.0f, 1.0f, 64.0f));
        _objects.append(createWall(14.0f, 32.0f, 8.0f, 1.0f, 0.0f*b2_pi));
        _objects.append(createWall(4.0f, 48.0f, 8.0f, 1.0f, 0.25f*b2_pi));
        _objects.append(createWall(24.0f, 48.0f, 8.0f, 1.0f, -0.25f*b2_pi));
    }

    Object createWall(float32 x, float32 y, float32 w, float32 h, float32 angle=0) {
        Object o;
        // body
        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
        bd.angle = angle * b2_pi;
        o.body = _world->CreateBody(&bd);
        // shape
        b2PolygonShape shape;
        shape.SetAsBox(w/2.0f, h/2.0f);
        // fixture
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 0.1f;
        fd.friction = 0.3f;
        o.fixture = o.body->CreateFixture(&fd);
        o.type = WallObject;
        return o;
    }

    Object createBall(const b2Vec2& pos, float32 radius) {
        Object o;
        // body
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position = pos;
        o.body = _world->CreateBody(&bd);
        // shape
        b2CircleShape shape;
        shape.m_radius = radius;
        // fixture
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 1.0f;
        fd.restitution = 0.6f;
        o.fixture = o.body->CreateFixture(&fd);
        o.type = BallObject;
        return o;
    }

    void paintEvent(QPaintEvent *) {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setTransform(_transform);
        foreach(const Object& o, _objects) {
            switch(o.type) {
            case BallObject:
                drawEllipse(&p, o);
                break;
            case WallObject:
                drawWall(&p, o);

            }

        }
    }

    void drawWall(QPainter *p, const Object& o) {
        float32 x = o.body->GetPosition().x;
        float32 y = o.body->GetPosition().y;
        float32 angle = o.body->GetAngle();
        const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(o.fixture->GetShape());
        float32 hx = shape->GetVertex(1).x;
        float32 hy = shape->GetVertex(2).y;
        QRectF r(x-hx, y-hy, 2*hx, 2*hy);
        p->save();
        p->translate(r.center());
        p->rotate(angle*180/b2_pi);
        p->translate(-r.center());
        p->drawRect(r);
        p->restore();
    }

    void drawEllipse(QPainter *p, const Object& o) {
        float32 x = o.body->GetPosition().x;
        float32 y = o.body->GetPosition().y;
        float32 r = o.fixture->GetShape()->m_radius;
        p->drawEllipse(QPointF(x, y), r, r);
    }

    void start() {
        if(!_timerId) {
            _timerId = startTimer(1000/60); // 60fps
        }
    }
    void timerEvent(QTimerEvent *event) {
        if(event->timerId() == _timerId) {
            _world->Step(1.0f/60.0f, 8, 3);
            update();
        }
    }

private:
    b2World *_world;
    int _timerId;
    QTransform _transform;
    QVector<Object> _objects;
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    World world;
    world.resize(360, 640);
    world.show();
    world.start();

    return app.exec();
}


