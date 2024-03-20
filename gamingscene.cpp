#include "gamingscene.h"
#include <QPainter>
#include "ComponentsCodes/playerplane.h"
#include <QDebug>
#include <QKeyEvent>
#include <Qt>
#include <QTimer>
#include "ConstValue.h"
#include "ComponentsCodes/bullet.h"
#include <cmath>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QFont>
#include "subwidget.h"

GamingScene::GamingScene(QWidget *parent) : QMainWindow(parent)
{

    /*没有这个语句的话点击按键后按键自动获得焦点
      则空格会响应按键*/
    setFocusPolicy(Qt::StrongFocus);

    //暂停时的ui
    stopHint=new QLabel(this);
    stopHint->setPixmap(QPixmap(":/Scene/Scene/pause_nor.png"));
    stopHint->move((WINDOW_WIDTH-stopHint->width())*0.5,(WINDOW_HEIGHT-stopHint->height())*0.5);
    stopHint->hide();

    restartBtn=new QPushButton(this);
    restartBtn->setText("重新开始");
    restartBtn->move((WINDOW_WIDTH-stopHint->width())*0.5,(WINDOW_HEIGHT-stopHint->height())*0.5+stopHint->height()+10);
    restartBtn->hide();

    /*还有一个返回菜单按钮*/
    returnToMenuBtn = new QPushButton(this);
    returnToMenuBtn->setText("返回主菜单");
    returnToMenuBtn->move((WINDOW_WIDTH-stopHint->width())*0.5,(WINDOW_HEIGHT-stopHint->height())*0.5+stopHint->height()+50);
    returnToMenuBtn->hide();
    //BGM
    QSound * bgm=new QSound(":/Sound/Sound/game_music.wav",this);
    bgm->setLoops(-1);
    bgm->play();
    //生命值
    healthTag[0]=new QLabel(this);
    healthTag[1]=new QLabel(this);
    healthTag[2]=new QLabel(this);
    healthTag[0]->setPixmap(QPixmap(":/Object/Object/life.png"));
    healthTag[0]->move(0,0);
    healthTag[1]->setPixmap(QPixmap(":/Object/Object/life.png"));
    healthTag[1]->move(60,0);
    healthTag[2]->setPixmap(QPixmap(":/Object/Object/life.png"));
    healthTag[2]->move(120,0);

    player=new PlayerPlane(":/Object/Object/me2.png",this);
    player->setPos(WINDOW_WIDTH*0.5-player->width()*0.5,WINDOW_HEIGHT*0.8);

    //计分板
    scoreLabel=new QLabel(this);
    QFont scoreFontSetting;
    scoreFontSetting.setPointSize(10);
    scoreLabel->move(0,healthTag[0]->height());
    scoreLabel->setText(QString("SCORE:%1").arg(score));

    //计分板更新信号
    connect(this,&GamingScene::updateScoreLabel,this,[=](){
       scoreLabel->setText(QString("SCORE:%1").arg(score));
    });

    //玩家生命信号
    connect(player,&PlayerPlane::healthDecrease,this,[=](){
        qDebug()<<"trigged";
        healthTag[healthTagTag++]->hide();
    });

    //炸弹数
    bombNumLabel = new QLabel(this);
    bombNumLabel->setText(QString("BombNum:%1").arg(player->getBombNum()));
    bombNumLabel->move(0,healthTag[0]->height()+scoreLabel->height());
    connect(player,&PlayerPlane::bombAdd,this,[=](){
        bombNumLabel->setText(QString("BombNum:%1").arg(player->getBombNum()));
    });
    //子弹数

    bulletNumLabel=new QLabel(this);
    bulletNumLabel->move(0,healthTag[0]->height()+scoreLabel->height()*2);
    bulletNumLabel->setText(QString("BulletNum:%1").arg(player->getBulletNum()));
    connect(player,&PlayerPlane::bulletAdd,this,[=](){
        qDebug()<<"Bullet Add";
        bulletNumLabel->setText(QString("BulletNum:%1").arg(player->getBulletNum()));
    });
    this->setWindowTitle("PlaneFight");
    this->setFixedSize(480,700);
    this->setWindowIcon(QIcon(":/Object/Object/me1.png"));

    updateTimer=new QTimer(this);//更新用定时器
    connect(updateTimer, &QTimer::timeout, this, &GamingScene::update);

    //生成玩家子弹
    bulletTimer = new QTimer(this);
    connect(bulletTimer, &QTimer::timeout, this, [=](){
        if(bulletFireCtrl && !updatingBullet){
            if(player->fireAtOnce()){
                Bullet * currentBullet1=new Bullet(Bullet::player,player->getX()+15,player->getY()-5,this);
                currentBullet1->show();
                bulletList.push_back(currentBullet1);

                Bullet * currentBullet2=new Bullet(Bullet::player,player->getX()+80,player->getY()-5,this);
                currentBullet2->show();
                bulletList.push_back(currentBullet2);
            }
        }
    });

    //生成Enemy测试
//    Enemy *testEnemy=new Enemy(1,this);
//    testEnemy->setPos(this->width()*0.5,0);
//    enemyList.push_back(testEnemy);

    //生成Enemy
    enemyGenerateTimer=new QTimer(this);
    connect(enemyGenerateTimer,&QTimer::timeout,this,&GamingScene::enemyGenerate);
    //enemyGenerateTimer->start(1000);

    connect(restartBtn,&QPushButton::clicked,this,[=](){
        restartGaming();
    });
    connect(returnToMenuBtn,&QPushButton::clicked,this,[=](){
       writescore(QString::number(score));
       subtest1.show();

       this->hide();
    });
}

void GamingScene::stopGaming(){
    this->updateTimerStop();
    enemyGenerateTimer->stop();

    //禁止键盘输入
    allowKeyInput=false;


    stopHint->show();
    restartBtn->show();
    returnToMenuBtn->show();
}

void GamingScene::continueGaming(){
    this->updateTimerStart();
    enemyGenerateTimer->start();

    allowKeyInput=true;

    stopHint->hide();
    restartBtn->hide();
    returnToMenuBtn->hide();
}

void GamingScene::restartGaming(){
    qDebug()<<"restart";
    writescore(QString::number(score));
    score=0;
    scoreLabel->setText(QString("SCORE:%1").arg(score));
    allowKeyInput=true;

    healthTag[0]->show();
    healthTag[1]->show();
    healthTag[2]->show();
    healthTagTag=0;

    //清除现有内容
    for (std::list<Enemy *>::iterator it=enemyList.begin();it!=enemyList.end();it++) {
        Enemy * currentEnemy=*it;
        currentEnemy->setVisible(false);
        enemyList.erase(it);
    }
    for (std::list<Bullet *>::iterator it = bulletList.begin(); it != bulletList.end(); it++){
        Bullet * currentBullet=*it;
        currentBullet->setVisible(false);
        bulletList.erase(it);
    }

    player->setVisible(false);
    player=new PlayerPlane(":/Object/Object/me2.png",this);
    player->setPos(WINDOW_WIDTH*0.5-player->width()*0.5,WINDOW_HEIGHT*0.8);
    player->show();

    stopHint->hide();
    restartBtn->hide();
    returnToMenuBtn->hide();

    updateTimerStart();
    enemyGenerateTimer->start();


    //重新连接和player有关的信号
    //玩家生命信号
    connect(player,&PlayerPlane::healthDecrease,this,[=](){
        qDebug()<<"trigged";
        healthTag[healthTagTag++]->hide();
    });

    connect(player,&PlayerPlane::bombAdd,this,[=](){
        bombNumLabel->setText(QString("BombNum:%1").arg(player->getBombNum()));
    });

    connect(player,&PlayerPlane::bulletAdd,this,[=](){
        bulletNumLabel->setText(QString("BulletNum:%1").arg(player->getBulletNum()));
    });
}

void GamingScene::update(){
    static int enemyUpdateCnt=0;

    //更新玩家
    player->move(player->getX(),player->getY());
    if(player->isTimeToDelete()){
        restartGaming();
        stopGaming();
    }
    if(checkCollide(player) || player->timeToExpode){
        /*敌人和玩家发生碰撞 || 敌人子弹和玩家发生碰撞*/
        player->timeToExpode=false;
        if(!player->haveBeenHit){
            if(!player->currentHealth()) allowKeyInput=false;//不然爆炸中还能操作
            //player->haveBeenHit=true;
            if(!player->CantHitMe) player->beenHit();
        }

    }


    //更新敌人
    updatingEnemy=true;
    enemyUpdateCnt++;
    if(enemyUpdateCnt==ENEMY_UPDATE_SPEED){
        for (std::list<Enemy *>::iterator it=enemyList.begin();it!=enemyList.end();it++) {
            Enemy * currentEnemy=*it;
            if(!currentEnemy->isTimeToDelete())
                currentEnemy->updateOnce(bulletList,updatingBullet,this);
            else{
                if(currentEnemy->returnType()==3) player->getBomb();
                if(currentEnemy->returnType()==4) player->getBullet();
                currentEnemy->setVisible(false);
                enemyList.erase(it);
            }
        }
        enemyUpdateCnt=0;
    }
    updatingEnemy=false;

    //更新子弹
    updatingBullet=true;

    for (std::list<Bullet *>::iterator it = bulletList.begin(); it != bulletList.end(); it++){
        Bullet * currentBullet=*it;
        if(!currentBullet->updateOnce() || checkCollide(currentBullet)){
            /*更新失败                       发生碰撞*/
            currentBullet->setVisible(false);
            bulletList.erase(it);//操作完再erase，qt可能自动释放了这些变量
        }
    }
    updatingBullet=false;
    //qDebug()<<bulletList.size();
}

void GamingScene::enemyGenerate(){
    int enemyGenerateSeed=QRandomGenerator::global()->bounded(1,101);
    int enemyPositionSeed=QRandomGenerator::global()->bounded(0,WINDOW_WIDTH-100);
    //qDebug()<<"enemyGenerateSeed:"<<enemyGenerateSeed;
    if(enemyList.size()<10){
        if(enemyGenerateSeed<50){
           //1/2概率生成敌人0
            Enemy *testEnemy=new Enemy(0,this);
            testEnemy->setPos(enemyPositionSeed,0);
            testEnemy->move(enemyPositionSeed,0);
            testEnemy->show();
            enemyList.push_back(testEnemy);
        }
        else if(enemyGenerateSeed<70){
            //1/5
            Enemy *testEnemy=new Enemy(1,this);
            testEnemy->setPos(enemyPositionSeed,0);
            testEnemy->move(enemyPositionSeed,0);
            testEnemy->show();
            enemyList.push_back(testEnemy);
        }
//        else if(enemyGenerateSeed<80){
//            //1/10
//            Enemy *testEnemy=new Enemy(2,this);
//            testEnemy->move(enemyPositionSeed,0);
//            enemyList.push_back(testEnemy);
//        }
    }

    //生成子弹和炸弹补给
    enemyGenerateSeed=QRandomGenerator::global()->bounded(1,21);
    enemyPositionSeed=QRandomGenerator::global()->bounded(0,WINDOW_WIDTH-100);
    if(enemyGenerateSeed==1){
        Enemy *testEnemy=new Enemy(3,this);
        testEnemy->setPos(enemyPositionSeed,0);
        testEnemy->move(enemyPositionSeed,0);
        testEnemy->show();
        enemyList.push_back(testEnemy);
    }else if (enemyGenerateSeed==2) {
        Enemy *testEnemy=new Enemy(4,this);
        testEnemy->setPos(enemyPositionSeed,0);
        testEnemy->move(enemyPositionSeed,0);
        testEnemy->show();
        enemyList.push_back(testEnemy);
    }
}

bool GamingScene::checkCollide(Bullet * bulletToBeCheck){
    int bulletMidX=bulletToBeCheck->getX()+bulletToBeCheck->width()*0.5;
    int bulletMidY=bulletToBeCheck->getY()+bulletToBeCheck->height()*0.5;
    switch (bulletToBeCheck->getSource()) {
        case Bullet::enemy:
        //判断敌人子弹与玩家碰撞
         {
            int playerMidX=player->getX()+player->width()*0.5;
            int playerMidY=player->getY()+player->height()*0.5;

            double dist=pow(pow(abs(bulletMidX-playerMidX),2)+pow(abs(bulletMidY-playerMidY),2),0.5);
            if(dist<std::min(player->width()*0.6,player->height()*0.6)){//
                //qDebug()<<"hit!";
                player->timeToExpode=true;
                return true;
            }
         }
        break;
        case Bullet::player:
        //判断玩家子弹与敌人碰撞
        for (std::list<Enemy *>::iterator it=enemyList.begin();it!=enemyList.end();it++) {
            Enemy * currentEnemy=*it;
            int enemyMidX=currentEnemy->getX()+currentEnemy->width()*0.5;
            int enemyMidY=currentEnemy->getY()+currentEnemy->height()*0.5;

            double dist=pow(pow(abs(bulletMidX-enemyMidX),2)+pow(abs(bulletMidY-enemyMidY),2),0.5);
            if(dist<std::min(currentEnemy->width(),currentEnemy->height())){//
                if(!currentEnemy->haveBeenHit){
                    //qDebug()<<"enemy hit!";
                    int typeNum=currentEnemy->returnType();
                    if(!(typeNum==3 || typeNum==4)) this->updateScore(typeNum+1);//击毁bonus不得分
                    currentEnemy->haveBeenHit=true;
                    currentEnemy->explode();
                    return true;
                }
            }
        }
        break;
    }
    return false;
}

bool GamingScene::checkCollide(PlayerPlane * playerToBeCheck){
    //判断玩家与敌人碰撞
    int playerMidX=playerToBeCheck->getX()+playerToBeCheck->width()*0.5;
    int playerMidY=playerToBeCheck->getY()+playerToBeCheck->height()*0.5;
    for (std::list<Enemy *>::iterator it=enemyList.begin();it!=enemyList.end();it++) {
        Enemy * currentEnemy=*it;
        int enemyMidX=currentEnemy->getX()+currentEnemy->width()*0.5;
        int enemyMidY=currentEnemy->getY()+currentEnemy->height()*0.5;

        double dist=pow(pow(abs(playerMidX-enemyMidX),2)+pow(abs(playerMidY-enemyMidY),2),0.5);
        if(dist<std::min(currentEnemy->width(),currentEnemy->height())){//
            qDebug()<<"enemy collide!";
            //碰撞也爆炸
            //this->updateScore(currentEnemy->returnType()+1);
            currentEnemy->haveBeenHit=true;
            currentEnemy->explode();
            return true;
        }
    }
    return false;
}

void GamingScene::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    QPixmap pBackGround(":/Scene/Scene/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pBackGround);
}

void GamingScene::keyPressEvent(QKeyEvent * event){
    if(allowKeyInput){
        switch (event->key()) {
            case Qt::Key_W:
                player->moveUp(PLAYER_SPEED);
                //qDebug()<<"W pressed";
            break;
            case Qt::Key_A:
                player->moveLeft(PLAYER_SPEED);
                //qDebug()<<"A pressed";
            break;
            case Qt::Key_S:
                player->moveDown(PLAYER_SPEED);
                //qDebug()<<"S pressed";
            break;
            case Qt::Key_D:
                player->moveRight(PLAYER_SPEED);
                //qDebug()<<"D pressed";
            break;
            case Qt::Key_P:
                this->stopGaming();
            break;
            case Qt::Key_G:
                if(player->useBomb())
                    this->bombEffect();
                bombNumLabel->setText(QString("BombNum:%1").arg(player->getBombNum()));
            break;
            case Qt::Key_Space:
                //qDebug()<<"wtf";
                bulletFireCtrl=!bulletFireCtrl;
                if(bulletFireCtrl) bulletTimerStart();
                else bulletTimerStop();
            break;
        }
        return;
    }
    else{
        if(event->key()==Qt::Key_R) restartGaming();
        if(event->key()==Qt::Key_P) continueGaming();
    }
    return QMainWindow::keyPressEvent(event);
}

void GamingScene::bombEffect(){
    for (std::list<Enemy *>::iterator it=enemyList.begin();it!=enemyList.end();it++) {
        Enemy * currentEnemy=*it;

        if(!currentEnemy->haveBeenHit){
            //qDebug()<<"enemy hit!";
            int typeNum=currentEnemy->returnType();
            if(!(typeNum==3 || typeNum==4)) this->updateScore(typeNum+1);//击毁bonus不得分
            currentEnemy->haveBeenHit=true;
            currentEnemy->explode();
        }
    }
}
