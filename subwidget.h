#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QString>
#include "leaderboard.h"

namespace Ui {
class subWidget;
}

class subWidget : public QWidget
{
    Q_OBJECT

public:
    explicit subWidget(QWidget *parent = nullptr);
    ~subWidget();

private:
    Ui::subWidget *ui;
    QMap<QString,int>map;
    leaderboard leader;
    void paintEvent(QPaintEvent *event);
};

#endif // SUBWIDGET_H
