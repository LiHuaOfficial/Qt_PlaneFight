#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include "subwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    subWidget subtest;

    //注册读取文件
    public:bool readRegister(QString str){
        QFile f("users.txt");
          if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
          {
              qDebug() << ("打开文件失败");
          }
          QTextStream txtInput(&f);
          QString lineStr,linestr2;
          while(!txtInput.atEnd())
          {
             lineStr = txtInput.readLine();
             linestr2 = txtInput.readLine();
             if(lineStr == str)return true;
         }

         f.close();
         return false;
    }
    //登录读取文件
    public:int readLogin(QString str,QString psw){
        QFile f("users.txt");
          if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
          {
              qDebug() << ("打开文件失败");
          }
          QTextStream txtInput(&f);
          QString lineStr,linestr2;
          while(!txtInput.atEnd())
          {
             lineStr = txtInput.readLine();
             linestr2 = txtInput.readLine();
             if(lineStr == str && linestr2 == psw)return 0;
             else if(lineStr == str && linestr2 != psw)return 1;
         }

         f.close();
         return 2;
    }

    //写入文件
    public: void writeuser(QString str){
        QFile f("users.txt");
        if(!f.open(QIODevice::ReadWrite | QIODevice::Append))   //以读写且追加的方式写入文本
         {
             qDebug() << ("打开文件失败");
         }
         QTextStream txtOutput(&f);
         txtOutput << str << endl;
         f.close();
    }

    //查找某用户
    public: bool searchUsername(QString str){
        return readRegister(str);
    }

    public: int searchUser(QString str,QString s){
        return readLogin(str,s);
    }

};
#endif // MAINWINDOW_H
