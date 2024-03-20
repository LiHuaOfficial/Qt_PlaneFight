#include "subwidget.h"
#include "ui_subwidget.h"
#include "leaderboard.h"
#include <QPainter>
#include "gamingscene.h"

subWidget::subWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::subWidget)
{
    ui->setupUi(this);
    //开始游戏
    connect(ui->play,&QPushButton::clicked,this,[=]{
            this->setWindowTitle("PlaneFight");
            this->setFixedSize(480,700);
            this->setWindowIcon(QIcon(":/Object/Object/me1.png"));

            //进入游戏界面

               GamingScene * gamingScene=new GamingScene(this);
               gamingScene->setGeometry(this->geometry());
               gamingScene->updateTimerStart();
               gamingScene->enemyGenTimerStart();
               gamingScene->show();
               this->hide();
    });
    //打开排行榜
    connect(ui->leaderBoard,&QPushButton::clicked,this,[=]{
        leader.show();
    });
}

subWidget::~subWidget()
{
    delete ui;
}
void subWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    QPixmap pBackGround(":/Scene/Scene/background.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pBackGround);

}
