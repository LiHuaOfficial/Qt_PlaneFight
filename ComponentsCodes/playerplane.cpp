#include "playerplane.h"
#include <QDebug>

PlayerPlane::PlayerPlane(QWidget *parent) : Plane(parent)
{

}

PlayerPlane::PlayerPlane(QString imgAddr,QWidget *parent) : Plane(imgAddr,parent)
{
    this->resize(100,100);

    //爆炸动画
    connect(imgUpdateTimer,&QTimer::timeout,this,[=](){
        if(explodeCnt<4){
            QPixmap * tmpImg=new QPixmap(explodeImg[explodeCnt++]);
            this->setImage(tmpImg);
        }
        else{
            timeToDelete=true;
        }
    });

    //爆炸音效
    explodeSound=new QSound(":/Sound/Sound/me_down.wav");
    explodeSound->setLoops(1);
}

void PlayerPlane::beenHit(){
    //haveBeenHit用于防止每一帧都调用这个函数
    if(health==0){
        haveBeenHit=true;
        explode();
    }
    else {
            health--;
            haveBeenHit=false;
            qDebug()<<"health :"<<health;
            emit this->healthDecrease();

            CantHitMe=true;
            //无敌时间
            QTimer::singleShot(1000,[=](){
                CantHitMe=false;
            });
    }
}

void PlayerPlane::explode(){
    qDebug()<<"explode start";
    explodeSound->play();
    imgUpdateTimer->start(DEATH_MOVIE_SPEED);
}
