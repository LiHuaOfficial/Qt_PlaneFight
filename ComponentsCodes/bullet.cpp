#include "bullet.h"

Bullet::Bullet(QWidget *parent) : Plane(":/Object/Object/bullet2.png",parent)
{
    this->resize(5,11);
}

Bullet::Bullet(enum BulletType fireSource,int X,int Y,QWidget *parent): Plane(":/Object/Object/bullet2.png",parent)
{
    currentType=fireSource;
    this->setPos(X,Y);
    this->move(X,Y);
}

bool Bullet::updateOnce(){
    switch (currentType) {
    case 0:
        return this->moveUp(BULLET_SPEED);
        break;
    case 1:
        return  this->moveDown(BULLET_SPEED);
        break;
    default:
        return false;
    }
}
