#include "drawingObject.h"
#ifndef POINT_H
#define POINT_H
class point:public drawingObject{
public:
    point( GLfloat r=1,GLfloat px=0.,GLfloat py=0.,GLfloat pz=0.,GLuint rsx=15,GLuint rsy=15 ):drawingObject(r,px,py,pz,rsx,rsy ){
     glNewList(shapeID, GL_COMPILE);
     shape();
     glEndList();
    }
    void shape(){
     float x1=-0.5,y1=-0.5;
     glBegin(GL_POINTS);
     glVertex2f(x1 + 0.5, y1 + 0.5);
     glColor3f(0,0,1);
     glEnd();
    }
};
#endif
 

