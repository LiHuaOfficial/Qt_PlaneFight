#include "plane.h"
#include <QPixmap>
Plane::Plane(QWidget *parent) : QLabel(parent)
{

}

Plane::Plane(QString imgAddr,QWidget *parent) : QLabel(parent){
    QPixmap *img=new QPixmap(imgAddr);
    this->resize(img->width(),img->height());
    this->setScaledContents(true);
    this->setPixmap(*img);

    x=0;y=0;


}

void Plane::setImage(QPixmap * img,bool remainSize){
    int currentWidth=this->width();
    int currentHeight=this->height();

    this->setPixmap(*img);
    if(remainSize) this->resize(currentWidth,currentHeight);
    else{
        this->resize(img->width(),img->height());
    }
};

void playMovie(std::vector<QPixmap> picArray,int ms){

}
