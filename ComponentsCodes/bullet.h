#ifndef BULLET_H
#define BULLET_H

#include <QWidget>
#include "ComponentsCodes/plane.h"

class Bullet : public Plane
{
    Q_OBJECT
public:
    enum BulletType{player=0,enemy=1};
    explicit Bullet(QWidget *parent = nullptr);
    explicit Bullet(enum BulletType fireSource,int X,int Y,QWidget *parent = nullptr);

    bool updateOnce();
    int getSource(){return currentType;};

    ~Bullet(){};

private:

    int currentType=2;
signals:

};

#endif // BULLET_H
