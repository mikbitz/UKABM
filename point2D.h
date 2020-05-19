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
public:
  point2D(){x=0;y=0;};
  point2D(double x_,double y_):x(x_),y(y_){};
  point2D(const point2D& p){x=p.x;y=p.y;}

   /**
   * @brief Check points for equality 
   * 
   * Points are only regarded as equal if both x any co-ordintes are strictly equal
   * 
   * @param p: Reference to another point to check against this one
   * @return bool
   */

  bool operator==(const point2D& p){return (x==p.x && y==p.y);}
  point2D operator+(const point2D& p){return point2D(x+p.x,y+p.y);}
  point2D operator-(const point2D& p){return point2D(x-p.x,y-p.y);}
  point2D operator-(const double& p){return point2D(x-p,y-p);}
  point2D operator/(const double& d){return point2D(x/d,y/d);}
  point2D operator*(const double& d){return point2D(x*d,y*d);}

  double size(){return sqrt(x*x+y*y);}
  double x;
  double y;
  int cellIndex;
};
#endif
