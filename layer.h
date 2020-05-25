#ifndef LAYER_H
#define LAYER_H
#include <string>
class extent3D{
    public:

double x,y,z;
extent3D(){x=0;y=0;z=0;}
extent3D(double x_,double y_,double z_):x(x_),y(y_),z(z_){}
void setExtent(double x_,double y_,double z_){x=x_;y=y_,z=z_;}
double getSize(){double a=x; if (y>a) a=y; if (z>a)a=z; return a;}
};
class layer{
public:
layer();
virtual bool update();
virtual extent3D getExtent();
virtual void     setExtent(double,double,double);
virtual double   getSize(){return extent.getSize();}
virtual void     output(){;}

void setName(std::string );

protected:
std::string name;
extent3D extent;
};
#endif
