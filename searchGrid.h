#ifndef SEARCHGRID_H
#define SEARCHGRID_H
#include <map>
#include <set>
#include <vector>
#include<string>
#include "point2D.h"
class agent;
class parameters;
using namespace std;
class searchGrid{
    vector<set<agent*> >cells;
    //x0 and y0 are the grid physical space origin
    //xSize and ysize the total physical extents of the domain (in metres, say)
    double x0,y0,xSize,ySize;
    //Numbers of cells in x and y direction.
    //so the cell size is xSize/NxCells in the xdirection etc.
    int NxCells,NyCells;
public:
    searchGrid();
    ~searchGrid();
    searchGrid(double,double,double,double,int,int);
    void init(double,double,double,double,int,int);
    void init();
    void resize(double,double,double,double,int,int);
    int  findCellIndex(double,double);
    void add(agent*);
    void remove(agent*);
    void eraseAll();
    void check();
    void check(agent*);
    void check(vector<agent*>&);
    bool dirty;
    void update();
    point2D getRandomPoint();
    vector <agent*> here(agent*);
    vector <agent*> inRadius(agent* ,double);
    vector <agent*> neighbours4(agent*);
    vector <agent*> neighbours(agent*);
    void   inCell(int,vector<agent*>&);
    void   inDist(int,float,agent*,vector<agent*>&);
    void wrapDefaults();
    void setToroidal();
    void setWrapX();
    void setWrapY();
    void setHardEdged();
    void setHardX();
    void setHardY();
    void setSphere();
    void setxChunk(int);
    void setyChunk(int);

    static void test();
private:
    static searchGrid* testGrid;
    void testMessage(string,bool);
    //functions and flags to deal with wrapping at grid edges
    bool toroidal,cylX,cylY,spheroidal,hardE,hX,hY,expanding,expX,expY;
    void wrapCoordinates(agent*);
    void torus(double&,double&);
    void cylinderX(double&);
    void cylinderY(double&);
    void sphere(double&,double&);
    void hardEdges(double&,double&);
    void hardX(double&);
    void hardY(double&);
};
#endif
