#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include "gamingscene.h"
#include <QDebug>
#include <QMessageBox>
#include "subwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //用户注册向文件中插入数据
    connect(ui->registerButton,&QPushButton::clicked,this,[=]{
        //接收用户输入
        QString user = ui->usernameLineEdit->text();
        QString psw = ui->password->text();
        if(!searchUsername(user)){
        //将用户信息存入文件中
        writeuser(user);
        writeuser(psw);

        QMessageBox::information(this,"提示","注册成功");
        }else {
            QMessageBox::information(this,"提示","该用户名已被注册");
        }
    });

    //用户登录
    connect(ui->loginButton,&QPushButton::clicked,this,[=]{
        QString user = ui->usernameLineEdit->text();
        QString psw = ui->password->text();
        if(searchUser(user,psw) == 0){
            QMessageBox::information(this,"提示","登录成功");
            subtest.show();
        }else if(searchUser(user,psw) == 2)
            QMessageBox::information(this,"提示","登录失败，请先注册账号");
        else QMessageBox::information(this,"提示","密码错误");
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

