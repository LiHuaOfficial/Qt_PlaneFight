#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QWidget>
#include <QString>
#include <QMap>
#include <QFile>
#include <QDebug>

namespace Ui {
class leaderboard;
}

class MyInt
{
public:
    //指定排序规则
    explicit MyInt(const int &value_) : value(value_) {}

    int value;
    //重载操作符
    bool operator < (const MyInt& other) const
    {
        return value > other.value;
    }
};

class leaderboard : public QWidget
{
    Q_OBJECT

public:
    explicit leaderboard(QWidget *parent = nullptr);
    ~leaderboard();

private:
    Ui::leaderboard *ui;
    QString score;
    QMap<MyInt,QString>map;

public:void readscore(){
    QFile f("user_scores.txt");
      if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      {
          qDebug() << ("打开文件失败");
      }
      QTextStream txtInput(&f);
      QString lineStr;
      while(!txtInput.atEnd())
      {
         lineStr = txtInput.readLine();
         map.insert(MyInt(lineStr.toInt()),"1");
     }
     f.close();
}
//写入排行榜
public:void wr(){
        QMapIterator<MyInt,QString>iter(map);
        int t = 1;
        while(iter.hasNext()){
            iter.next();
            score += "排名：";
            score += QString::number(t);
            score += " 分数：";
            score += QString::number(iter.key().value);
            score += "\n";
            t++;
        }
    }
};

#endif // LEADERBOARD_H
