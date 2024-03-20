#ifndef PLAYERPLANE_H
#define PLAYERPLANE_H

#include <QWidget>
#include <QPixmap>
#include "plane.h"
#include <QTimer>
#include <QDebug>

class PlayerPlane : public Plane
{
    Q_OBJECT
public:
    bool timeToExpode=false;
    bool haveBeenHit=false;//防止explode()多次调用
    bool CantHitMe=false;

    explicit PlayerPlane(QWidget *parent = nullptr);
    explicit PlayerPlane(QString imgAddr,QWidget *parent = nullptr);

    bool moveUp(int offset){
        this->setImage(pMoveFront);
        if(y-offset>0){
            y-=offset;
            return true;
        }
        else return false;
    };
    bool moveDown(int offset){
        this->setImage(pMoveElse);
        if(y+offset<WINDOW_HEIGHT-this->height()){
            y+=offset;
            return true;
        }
        else return false;
    };
    bool moveLeft(int offset){
        this->setImage(pMoveElse);
        if(x-offset>0){
            x-=offset;
            return true;
        }
        else return false;
    };
    bool moveRight(int offset){
        this->setImage(pMoveElse);
        if(x+offset<WINDOW_WIDTH-this->width()){
            x+=offset;
            return true;
        }
        else return false;
    };

    void explode();
    void beenHit();

    bool isTimeToDelete(){return timeToDelete;};

    int currentHealth(){return health;};

    void getBomb(){bombNum++;qDebug()<<"bomb get!";emit this->bombAdd();};
    void getBullet(){BulletNum+=20;qDebug()<<"bullet get!";emit this->bulletAdd();};

    int getBombNum(){return bombNum;};
    int getBulletNum(){return BulletNum;};

    bool useBomb(){
        if(bombNum) {bombNum--;return true;}
        else return false;
    };
    bool fireAtOnce(){
        if(BulletNum>=2){
            BulletNum-=2;
            emit this->bulletAdd();
            return true;
        }
        else{
            return false;
        }
    }
private:
    QPixmap * pMoveFront=new QPixmap(":/Object/Object/me1.png");
    QPixmap * pMoveElse=new QPixmap(":/Object/Object/me2.png");

    //爆炸相关
    const QString explodeImg[4]={":/Object/Object/me_destroy_1.png",
                                ":/Object/Object/me_destroy_2.png",
                                ":/Object/Object/me_destroy_3.png",
                                ":/Object/Object/me_destroy_4.png" };


    bool timeToDelete=false;

    int explodeCnt=0;

    QTimer * imgUpdateTimer=new QTimer(this);

    QSound * explodeSound;

    //生命值相关
    int health=3;

    //bonus获取
    int bombNum=0;
    int BulletNum=40;
signals:
    void healthDecrease();
    void bombAdd();
    void bulletAdd();
};

#endif // PLAYERPLANE_H
