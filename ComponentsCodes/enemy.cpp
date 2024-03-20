#include "enemy.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QPixmap>
#include <QSound>

Enemy::Enemy(QWidget *parent) : Plane(parent)
{

}

Enemy::Enemy(int enemyType,QWidget *parent) :Plane(":/Object/Object/enemy1.png",parent)
{
    currentEnemyType=enemyType;
    QPixmap *enemyImg=new QPixmap(enemyImgAddr[enemyType]);
    this->setImage(enemyImg,false);//图片不同大小也不同

    //音效
    explodeSound=new QSound(":/Sound/Sound/enemy1_down.wav",this);
    explodeSound->setLoops(1);

    bonusGetSound[0]=new QSound(":/Sound/Sound/get_bomb.wav",this);
    bonusGetSound[1]=new QSound(":/Sound/Sound/get_bullet.wav",this);
    bonusGetSound[0]->setLoops(1);
    bonusGetSound[1]->setLoops(1);

    //爆炸动画
    connect(imgUpdateTimer,&QTimer::timeout,this,[=](){
        if(explodeCnt<4){
            QPixmap * tmpImg=new QPixmap(explodeImg[currentEnemyType][explodeCnt++]);
            this->setImage(tmpImg);
        }
        else{
            timeToDelete=true;
        }
    });
}

void Enemy::updateOnce(std::list<Bullet *> & bulletList,bool & updatingBullet,QWidget *bulletParent){
    //随机发射子弹
    int bulletGenerateSeed=QRandomGenerator::global()->bounded(1,200);
    //qDebug()<<bulletGenerateSeed;
    if(bulletGenerateSeed==1 && currentEnemyType==1){//只有1号敌人发射子弹
        //1/100概率发射一次子弹
        qDebug()<<"Bullet Fired by "<<currentEnemyType;
        Bullet * currentBullet=new Bullet(Bullet::enemy,this->getX()+this->width()*0.5,this->getY()+this->height(),bulletParent);
        currentBullet->show();
        bulletList.push_back(currentBullet);
    }

    //随机移动
    bool haveMoved=false;
    do{
        int moveSeed=QRandomGenerator::global()->bounded(1,7);
        if(1<=moveSeed && moveSeed<=2 && this->getY()<WINDOW_HEIGHT*0.5) haveMoved=this->moveDown(ENEMY_SPEED);
        if(3<=moveSeed && moveSeed<=4) haveMoved=this->moveLeft(ENEMY_SPEED);
        if(5<=moveSeed && moveSeed<=6) haveMoved=this->moveRight(ENEMY_SPEED);
        if(moveSeed==7) haveMoved=this->moveUp(ENEMY_SPEED);
    }
    while (!haveMoved);
}

void Enemy::explode(){
    if(!(currentEnemyType==3 || currentEnemyType==4)){
        explodeSound->play();
        imgUpdateTimer->start(DEATH_MOVIE_SPEED);
    }
    else{
        bonusGetSound[currentEnemyType-3]->play();
        timeToDelete=true;

        /*为了让两个bonus被击毁时影响到player
          实现更简单的方法应该是析构enemy类时（也就是在GamingScene中）
          判断enemy种类并发出信号*/

//        if(currentEnemyType==3) ;
//        if(currentEnemyType==4) ;
    }
}
