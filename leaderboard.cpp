#include "leaderboard.h"
#include "ui_leaderboard.h"

leaderboard::leaderboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::leaderboard)
{
    ui->setupUi(this);
    connect(ui->sort,&QPushButton::clicked,this,[=]{
        readscore();
        wr();
        ui->textEdit->setText(score);
    });

}

leaderboard::~leaderboard()
{
    delete ui;
}
