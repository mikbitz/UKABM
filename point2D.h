#ifndef point2D_H
#define point2D_H
#include <math.h>
/**
 * @class point2D
 * 
 * @brief A class to hold pairs of x,y co-ordinates
 * 
 **/
class point2D{
  double _x;
  double _y;
public:
  point2D(){_x=0;_y=0;};
  point2D(double x_,double y_):_x(x_),_y(y_){};
  point2D(const point2D& p){_x=p._x;_y=p._y;}

   /**
   * @brief Check points for equality 
   * 
   * Points are only regarded as equal if both x any co-ordintes are strictly equal
   * 
   * @param p: Reference to another point to check against this one
   * @return bool
   */

  bool operator==(const point2D& p){return (_x==p._x && _y==p._y);}
  point2D operator+(const point2D& p){return point2D(_x+p._x,_y+p._y);}
  point2D operator-(const point2D& p){return point2D(_x-p._x,_y-p._y);}
  point2D operator-(const double& p){return point2D(_x-p,_y-p);}
  point2D operator/(const double& d){return point2D(_x/d,_y/d);}
  point2D operator*(const double& d){return point2D(_x*d,_y*d);}

  double size(){return sqrt(_x*_x+_y*_y);}
  double  X(){return _x;}
  double  Y(){return _y;}
  void setX(double x){_x=x;}
  void setY(double y){_y=y;}

};
#endif
