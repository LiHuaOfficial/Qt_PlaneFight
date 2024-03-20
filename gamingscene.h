#ifndef GAMINGSCENE_H
#define GAMINGSCENE_H

#include <QMainWindow>
#include <QTimer>
#include "ComponentsCodes/playerplane.h"
#include <list>
#include "ComponentsCodes/bullet.h"
#include "ComponentsCodes/enemy.h"
#include <QPushButton>
#include "subwidget.h"

class GamingScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit GamingScene(QWidget *parent = nullptr);

    void updateTimerStart(){updateTimer->start(16);};
    void updateTimerStop(){updateTimer->stop();};

    void bulletTimerStart(){bulletTimer->start(700);};
    void bulletTimerStop(){bulletTimer->stop();};

    void enemyGenTimerStart(){enemyGenerateTimer->start(1000);};

    void restartGaming();
    void stopGaming();
    void continueGaming();
    //写入分数
    public: void writescore(QString str){
        QFile f("user_scores.txt");
        if(!f.open(QIODevice::ReadWrite | QIODevice::Append))   //以读写且追加的方式写入文本
         {
             qDebug() << ("打开文件失败");
         }
         QTextStream txtOutput(&f);
         txtOutput << str << endl;
         f.close();
    }
private:
    QTimer * updateTimer;
    subWidget subtest1;
    //关于玩家
    PlayerPlane * player;

    //关于子弹
    bool bulletFireCtrl=false;
    bool updatingBullet=false;
    std::list <Bullet *> bulletList;
    QTimer *bulletTimer;

    //关于敌人
    bool updatingEnemy=false;
    std::list<Enemy *> enemyList;

    QTimer * enemyGenerateTimer;

    //操作控制
    bool allowKeyInput=true;


    //暂停时显示的内容
    QLabel * stopHint;
    QPushButton * restartBtn;
    QPushButton * returnToMenuBtn;

    //UI
    QLabel * healthTag[3];
    int healthTagTag=0;

    QLabel * scoreLabel;
    unsigned long long score=0;

    QLabel * bombNumLabel;
    QLabel * bulletNumLabel;

    void updateScore(int newScore){
        score+=newScore;
        emit this->updateScoreLabel();
    };

    bool checkCollide(Bullet * bulletToBeCheck);
    bool checkCollide(PlayerPlane * playerToBeCheck);

    void update();
    void enemyGenerate();


    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent * event);

    void bombEffect();
signals:
    void generateBullet();
    void updateScoreLabel();
};

#endif // GAMINGSCENE_H
