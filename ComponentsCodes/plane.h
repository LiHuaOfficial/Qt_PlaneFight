#ifndef PLANE_H
#define PLANE_H

#include <QLabel>
#include "ConstValue.h"
#include <vector>
#include <QSound>

class Plane : public QLabel
{
    Q_OBJECT
public:
    explicit Plane(QWidget *parent = nullptr);
    explicit Plane(QString imgAddr,QWidget *parent = nullptr);

    void planeInit(){};

    int getX(){return x;};
    int getY(){return y;};
    void setPos(int posX,int posY){x=posX;y=posY;};

    /* 480*700 */
    virtual bool moveUp(int offset){
        if(y-offset>0){
            y-=offset;
            myMove();
            return true;
        }
        else return false;
    };
    virtual bool moveDown(int offset){
        if(y+offset<WINDOW_HEIGHT-this->height()){
            y+=offset;
            myMove();
            return true;
        }
        else return false;
    };
    virtual bool moveLeft(int offset){
        if(x-offset>0){
            x-=offset;
            myMove();
            return true;
        }
        else return false;
    };
    virtual bool moveRight(int offset){
        if(x+offset<WINDOW_WIDTH-this->width()){
            x+=offset;
            myMove();
            return true;
        }
        else return false;
    };

protected:
    int x,y;

    void myMove(){this->move(x,y);};
    void setImage(QPixmap * img,bool remainSize=true);
    void playMovie(std::vector<QPixmap> picArray,int ms);

signals:

};

#endif // PLANE_H
