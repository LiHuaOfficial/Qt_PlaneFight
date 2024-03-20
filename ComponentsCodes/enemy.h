#ifndef ENEMY_H
#define ENEMY_H

#include "plane.h"
#include "bullet.h"
#include <QTimer>
#include <QSound>


class Enemy : public Plane
{
    Q_OBJECT
public:
    enum enemyType{enemy1,enemy2,enemy3};

    bool haveBeenHit=false;
    explicit Enemy(QWidget *parent = nullptr);
    explicit Enemy(int enemyType,QWidget *parent= nullptr);


    void updateOnce(std::list <Bullet *> & bulletList,bool & updatingBullet,QWidget *bulletParent);
    void explode();
    bool isTimeToDelete(){return timeToDelete;};
    int returnType(){return currentEnemyType;};
private:
    const QString enemyImgAddr[5]={":/Object/Object/enemy1.png",
                                   ":/Object/Object/enemy2.png",
                                   ":/Object/Object/enemy3_n1.png",
                                   ":/Object/Object/bomb_supply.png",
                                   ":/Object/Object/bullet_supply.png"};

    int currentEnemyType;

    //爆炸画面
    const QString explodeImg[3][4]={  {":/Object/Object/enemy1_down1.png",
                                ":/Object/Object/enemy1_down2.png",
                                ":/Object/Object/enemy1_down3.png",
                                ":/Object/Object/enemy1_down4.png"} ,

                               {":/Object/Object/enemy2_down1.png",
                                ":/Object/Object/enemy2_down2.png",
                                ":/Object/Object/enemy2_down3.png",
                                ":/Object/Object/enemy2_down4.png"} ,

                               {":/Object/Object/enemy3_down1.png",
                                ":/Object/Object/enemy3_down2.png",
                                ":/Object/Object/enemy3_down3.png",
                                ":/Object/Object/enemy3_down4.png"} };

    bool timeToDelete=false;

    int explodeCnt=0;

    QTimer * imgUpdateTimer=new QTimer(this);
    QSound * explodeSound;

    //爆炸音效
    //QSound * explodeSound[3];

    //两个增益
    QSound * bonusGetSound[2];

signals:

};

#endif // ENEMY_H
