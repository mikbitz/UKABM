#include "searchGrid.h"
#include "agent.h"
//TODO Fix distance measures on the sphere and searches for neighbours at the poles
//------------------------------------------------------------------
//Destructor
//------------------------------------------------------------------
searchGrid::~searchGrid(){
    eraseAll();
}
//------------------------------------------------------------------
//Initialization
//------------------------------------------------------------------
searchGrid::searchGrid(){
    x0=-500;y0=-500;_xSize=1000.;_ySize=1000.;NxCells=1000;NyCells=1000;
    dirty=false;
    cells.resize(NxCells*NyCells);
    wrapDefaults();
}
//------------------------------------------------------------------
searchGrid::searchGrid(double x0_,
                       double y0_,
                       double xSize_,
                       double ySize_,
                       int    NxCells_,
                       int    NyCells_):
                       x0(x0_),
                       y0(y0_),
                       _xSize(xSize_),
                       _ySize(ySize_),
                       NxCells(NxCells_),
                       NyCells(NyCells_)                       
{
   dirty=false;
   cells.resize(NxCells*NyCells);
   wrapDefaults();
}
//------------------------------------------------------------------
void searchGrid::init(double x0_,
                      double y0_,
                      double xSize_,
                      double ySize_,
                      int    NxCells_,
                      int    NyCells_)
{
                       x0=x0_;
                       y0=y0_;
                       _xSize=xSize_;
                       _ySize=ySize_;
                       NxCells=NxCells_;
                       NyCells=NyCells_;                       
                       dirty=false;
                       cells.resize(NxCells*NyCells);
                       wrapDefaults();
}
//------------------------------------------------------------------
void searchGrid::init()
{
    parameters& p=parameters::getInstance();
    init(p.x0,p.y0,p.xSize,p.ySize,p.NxCells,p.NyCells);
    wrapDefaults();
}
//------------------------------------------------------------------
//kind of a dangerous function! 
//Particles that fall outside the grid will get squidged to edges or wrapped.
void searchGrid::resize(double x0_,
                   double y0_,
                   double xSize_,
                   double ySize_,
                   int    NxCells_,
                   int    NyCells_)
{
                   x0=x0_;
                   y0=y0_;
                   _xSize=xSize_;
                   _ySize=ySize_;
                   NxCells=NxCells_;
                   NyCells=NyCells_;
                   cells.resize(NxCells*NyCells);
                   check();
}
//------------------------------------------------------------------
void searchGrid::update()
//for expanding grids we need to re-calculate all cell indices if anything drifted past the boundary
{
   if (dirty)check();
}
//------------------------------------------------------------------
// Set up the functions that deal with agents that try to move outside the grid
//------------------------------------------------------------------
void searchGrid::wrapDefaults(){
    //default is to not allow agents to go beyond the grid edge
        toroidal   = false;
        cylX       = false;
        cylY       = false;
        spheroidal = false;
        hardE      = true;
        hX         = true;
        hY         = true;

}
//------------------------------------------------------------------
void searchGrid::setToroidal() {wrapDefaults();toroidal=true;setWrapX();setWrapY();}
void searchGrid::setWrapX()    {cylX=true;hX=false;hardE=false;spheroidal=false;}
void searchGrid::setWrapY()    {cylY=true;hY=false;hardE=false;spheroidal=false;}
void searchGrid::setHardEdged(){wrapDefaults();}
void searchGrid::setHardX()    {cylX=false;hX=true;toroidal=false;spheroidal=false;}
void searchGrid::setHardY()    {cylY=false;hY=true;toroidal=false;spheroidal=false;}
void searchGrid::setSphere()   {wrapDefaults();setWrapX();spheroidal=true;}
//------------------------------------------------------------------
void searchGrid::wrapCoordinates(agent* a){
    
    if (spheroidal){sphere   (a->loc.x,a->loc.y);}
    if (toroidal)  {torus    (a->loc.x,a->loc.y);}
    else{
        if (cylX)      {cylinderX(a->loc.x)      ;}
        if (cylY)      {cylinderY(a->loc.y)      ;}
    }  
    
    if (hardE)     {hardEdges(a->loc.x,a->loc.y);}
    else{
        if (hX)        {hardX    (a->loc.x)      ;}
        if (hY)        {hardY    (a->loc.y)      ;}
    }
}

//------------------------------------------------------------------
void searchGrid::torus(double& x, double& y){
    cylinderX(x);
    cylinderY(y);
}
//------------------------------------------------------------------
void searchGrid::cylinderX(double& x){
    while (x >  _xSize+x0)x=x-_xSize;
    while (x <= x0)      x=x+_xSize;
}
//------------------------------------------------------------------
void searchGrid::cylinderY(double& y){
    while (y >  _ySize+y0)y=y-_ySize;
    while (y <= y0)      y=y+_ySize;
}
//------------------------------------------------------------------
void searchGrid::sphere(double& x,double& y){
    if (y > _ySize+y0){y=_ySize+y0-(y-y0-_ySize);x=x+_xSize/2;}
    if (y < y0)      {y=y0      -(y-y0);      x=x+_xSize/2;}
    cylinderX(x);
}//------------------------------------------------------------------
void searchGrid::hardEdges(double& x,double& y){
    hardX(x);
    hardY(y);
}
//------------------------------------------------------------------
void searchGrid::hardX(double& x){
    if (x > _xSize+x0)x=_xSize+x0;
    if (x < x0)      x=x0;
}
//------------------------------------------------------------------
void searchGrid::hardY(double& y){
    if (y > _ySize+y0)y=_ySize+y0;
    if (y < y0)      y=y0;
}
//------------------------------------------------------------------
// Add and remove agents
//------------------------------------------------------------------
int searchGrid::findCellIndex(double x,double y){
     // find the raster indices of bucket containing x,y
    int ix = static_cast<int>((x-x0) /_xSize * NxCells);
    int iy = static_cast<int>((y-y0) /_ySize * NyCells);
    //stop agents exactly on the extreme right boundary (or overflowing it) from wrapping around into the next row up
    if (ix==NxCells)ix--;
    //similarly make sure we don't overflow in y
    if (iy==NyCells)iy--;
    //make sure we are n't below zero
    if (ix<0)ix=0;
    if (iy<0)iy=0;
    // return the bucket's linear index
    return iy * NxCells + ix;
}
//------------------------------------------------------------------
void searchGrid::add(agent* a){
    //Before adding any agents check to see they are not already in the grid.
    //This guards against any agent being moved and then added a second time into a different cell
    //(at which point it would awkward to find the first instance, as the calculated cellindex
    //would always point at the second cell...)
    //Cellindex default in agent constructor makes a->cellIndex -1 if not yet in the grid
    remove(a);
    //as a matter of policy force the agent into the grid
    //this might not be appropriate for other kinds of grid, but this one is assumed to define the model domain.
    wrapCoordinates(a);
    a->cellIndex=findCellIndex(a->loc.x,a->loc.y);
    cells[a->cellIndex].insert(a);
}
//------------------------------------------------------------------
void searchGrid::remove(agent* a){
    if (a->cellIndex!=-1)cells[a->cellIndex].erase(a);
}
//------------------------------------------------------------------
void searchGrid::eraseAll(){
    //loop over all (occupied) cells and remove every agent 
    vector<agent*> temp;
    for (auto& c:cells){//each cell is a set of pointers to agents
        for (auto& a:c){//loop over the set - note agents are not destroyed!
            remove(a);
        }
    }
}
//------------------------------------------------------------------
//Test functions to make sure agents are in the correct cells
//------------------------------------------------------------------
void searchGrid::check(){
    //loop over all (occupied) cells and check every agent to see whether it should still be in the cell
    //If not add it to a list to be re-allocated
    vector<agent*> temp;
    for (auto& c:cells){
        for (auto& a:c){
            wrapCoordinates(a);
            if (findCellIndex(a->loc.x,a->loc.y) != a->cellIndex) {
                 temp.push_back(a);
            }
        }
    }
    //re-allocate all mis-celled agents -note that add will remove them first...
    //this ensures agents are never present in the grid twice 
    for (unsigned i=0;i<temp.size();++i){
        add(temp[i]);
    }
    dirty=false;
}
//------------------------------------------------------------------
void searchGrid::check(vector<agent*>& ags){
    //loop over all (occupied) cells and check every agent to see whether it should still be in the cell
    //If not add it to a list to be re-allocated
    vector<agent*> temp;
    for (auto& a:ags){
        wrapCoordinates(a);
        if (findCellIndex(a->loc.x,a->loc.y) != a->cellIndex) {
            temp.push_back(a);
        }
    }
    
    //re-allocate all mis-celled agents -note that add will remove them first...
    //this ensures agents are never present in the grid twice 
    for (unsigned i=0;i<temp.size();++i){
        add(temp[i]);
    }
    dirty=false;
}
//------------------------------------------------------------------
void searchGrid::check(agent* a){

   wrapCoordinates(a);
   if (findCellIndex(a->loc.x,a->loc.y) != a->cellIndex){
        add(a);
   }
}
//------------------------------------------------------------------
//Neighbour finding - sphere not dealt with correctly yet (at poles, and for distances)!!
//------------------------------------------------------------------
vector <agent*> searchGrid::here(agent* a){
    vector<agent*> temp;
    int q=a->cellIndex;
    inCell(q,temp);
    return temp;
}
//------------------------------------------------------------------
vector <agent*> searchGrid::neighbours4(agent* a){
    vector<agent*> temp;
    int q=a->cellIndex;
    int il=-1,iu=1,jl=-1,ju=1;
    int lx=q%NxCells; int ly=q/NxCells;
    
    //hard edges
    if (!cylX) {if (lx==0) il=0;if (lx==NxCells-1) iu=0;}
    if (!cylY) {if (ly==0) jl=0;if (ly==NyCells-1) ju=0;}

    for (int i=il;i<iu+1;i++) {
        //wrapping
        int inx=lx+i; if (cylX){while (inx<0)inx+=NxCells; while (inx>=NxCells)inx-=NxCells;}
        int ind=inx+ly*NxCells;
        if (ind>=0 && ind< (int)cells.size()) inCell(ind,temp);
    }

    for (int j=jl;j<ju+1;j+=2) {
        //wrapping
        int iny=ly+j; if (cylY) {while (iny<0)iny+=NyCells; while (iny>=NyCells)iny-=NyCells;}
        int ind=lx+iny*NxCells;
        if (ind>=0 && ind< (int)cells.size()) inCell(ind,temp);
      }

    return temp;
}
//------------------------------------------------------------------
vector <agent*> searchGrid::neighbours(agent* a){
    vector<agent*> temp;
    int q=a->cellIndex;
    int il=-1,iu=1,jl=-1,ju=1;
    int ly=q/NxCells;int lx=q-ly*NxCells; 
    
    //hard edges
    if (!cylX) {if (lx==0) il=0;if (lx==NxCells-1) iu=0;}
    if (!cylY) {if (ly==0) jl=0;if (ly==NyCells-1) ju=0;}

    for (int i=il;i<iu+1;i++) {
      for (int j=jl;j<ju+1;j++) {
        //wrapping
        int inx=lx+i; if (cylX) {while (inx<0)inx+=NxCells; while (inx>=NxCells)inx-=NxCells;}
        int iny=ly+j; if (cylY) {while (iny<0)iny+=NyCells; while (iny>=NyCells)iny-=NyCells;}
        int ind=inx+iny*NxCells;
        if (ind>=0 && ind< (int)cells.size()) inCell(ind,temp);
      }
    }
    return temp;
}
//------------------------------------------------------------------
void   searchGrid::inCell(int q,vector<agent*>& L){
    set<agent*>::iterator s;
    for (s=cells[q].begin();s!=cells[q].end();++s){
        L.push_back(*s);
    }
}
//------------------------------------------------------------------
vector <agent*> searchGrid::inRadius(agent* a, double d){
    //find all agents within a given distance of agent a
    //may need to search multiple cells
    vector<agent*> temp;

    int q=a->cellIndex;
    int lx=q%NxCells; int ly=q/NxCells;
    int nx=int(d/_xSize*NxCells+0.5);
    int ny=int(d/_ySize*NyCells+0.5);
    int il=-nx,iu=nx,jl=-nx,ju=nx;
    //The domain is spanned! avoid duplications
    if (nx>=NxCells/2) {il=-lx;iu=NxCells-1-lx;}
    if (ny>=NyCells/2) {jl=-ly;ju=NyCells-1-ly;}
    //hard edges
    if (!cylX) {if (lx<nx) il=-lx;if (lx>=NxCells-1-nx) iu=NxCells-1-lx;}
    if (!cylY) {if (ly<ny) jl=-ly;if (ly>=NyCells-1-ny) ju=NyCells-1-ly;}
    
    for (int i=il;i<iu+1;i++)  {
      for (int j=jl;j<ju+1;j++) {
        //wrapping
        int inx=lx+i; if (cylX) {while (inx<0)inx+=NxCells; while (inx>=NxCells)inx-=NxCells;}
        int iny=ly+j; if (cylY) {while (iny<0)iny+=NyCells; while (iny>=NyCells)iny-=NyCells;}
        int ind=inx+iny*NxCells;
        if(ind>=0 && ind< (int)cells.size()) inDist(ind,d,a,temp);
      }
    }
  return temp;
}
//------------------------------------------------------------------
//return all agents in a cell within a given distance of agent a
void   searchGrid::inDist(int q,float d,agent* a,vector<agent*>& L){
    set<agent*>::iterator s;
    for (s=cells[q].begin();s!=cells[q].end();++s){
        agent* t=*s;
        double dx=(t->loc.x - a->loc.x);
        double dy=(t->loc.y - a->loc.y);
        //wrapping of distance measures
        if (cylX && abs(dx)>_xSize/2)dx=_xSize-abs(dx);
        if (cylY && abs(dy)>_ySize/2)dy=_ySize-abs(dy);
        if (dx*dx+dy*dy <= d*d)L.push_back(t);
      }
}
//------------------------------------------------------------------
vector <agent*> searchGrid::inSquareRegion(double x, double y, double d){
    //find all agents within a given square of size d with bottom left location of (x,y)
    vector<agent*> temp;
    //The domain is spanned! avoid duplications
    if (d>_xSize)d=_xSize;
    if (d>_ySize)d=_ySize;
    //get search indices
    int il=(x-x0)/_xSize*NxCells; int jl=(y-y0)/_ySize*NyCells;
    int iu=il + d/_xSize*NxCells+1; int ju=jl + d/_ySize*NyCells+1;

    //hard edges
    if (!cylX) {if (il<0) il=0;if (iu>=NxCells) iu=NxCells-1;}
    if (!cylY) {if (jl<0) jl=0;if (ju>=NyCells) ju=NyCells-1;}
    //cylinder wrap
    if (cylX) {while (il<0){il+=NxCells;x=x+_xSize;}}
    if (cylY) {while (jl<0){jl+=NyCells;y=y+_ySize;}}
    for (int i=il;i<iu+1;i++)  {
      for (int j=jl;j<ju+1;j++) {
        int inx=i;
        int iny=j;
        //wrapping
        if (cylX) {
            if (inx>=NxCells){inx-=NxCells;d=d-(x0+_xSize-x);}
        }
 
        if (cylY) {
            if (iny>=NyCells){iny-=NyCells;d=d-(y0+_ySize-y);}
        }
        int ind=inx+iny*NxCells;
        if(ind>=0 && ind< (int)cells.size()) inSquare(ind,x,y,x+d,y+d,temp);

      }

    }
  return temp;
}
//------------------------------------------------------------------
//return all agents in a cell within a given area bounded by x,xd,y,yd
void   searchGrid::inSquare(unsigned q,double x,double y,double xd, double yd,vector<agent*>& L){
    set<agent*>::iterator s;
    for (s=cells[q].begin();s!=cells[q].end();++s){
        agent* t=*s;
        double ax=t->loc.x;
        double ay=t->loc.y;
        //check whether in range allowing for exact upper and right boundary
        if (ax>=x && ay>=y && ((ax<xd) || (ax==(x0+_xSize) && (ax==xd)) ) && ((ay<yd) || (ay==(y0+_ySize)  && (ay==yd)) ) )L.push_back(t);
      }
}
//------------------------------------------------------------------
double searchGrid::xOrigin(){return x0;}
double searchGrid::yOrigin(){return y0;}
double searchGrid::xSize(){return _xSize;}
double searchGrid::ySize(){return _ySize;}
//------------------------------------------------------------------
point2D   searchGrid::getRandomPoint(){
float x=x0+_xSize*model::getInstance().random.number();
float y=y0+_ySize*model::getInstance().random.number();
return point2D(x,y);
}
//------------------------------------------------------------------
//return a grid file writer if the current grid is exactly uniform in x/y cellsize
asciiGridFileWriter* searchGrid::getAsciiFileWriter(const std::string& filePath, double missing){
    //this function only works for a regular grid (i.e. x and y spacing equal)
    assert(_xSize/NxCells==_ySize/NyCells);
    return new asciiGridFileWriter(filePath,NxCells,NyCells,x0,y0,_xSize/NxCells,missing);
}
//------------------------------------------------------------------
//return a grid file writer with a given cellsize, spanning the whole of this grid
asciiGridFileWriter* searchGrid::getAsciiFileWriter(const std::string& filePath,double cellSize,double missing){
    //this function only works in conjunction with regular grid (i.e. x and y spacing equal)
    //use only with count functions that take this into account.
    assert(_xSize/cellSize>0 && _ySize/cellSize>0);
    return new asciiGridFileWriter(filePath,_xSize/cellSize,_ySize/cellSize,x0,y0,cellSize,missing);
}
//------------------------------------------------------------------
//return a grid file writer with a given cellsize, spanning a subrange of the grid
asciiGridFileWriter* searchGrid::getAsciiFileWriter(const std::string& filePath,double cellSize,double xlo,double xhi,double ylo,double yhi,double missing){
    //this function only works in conjunction with regular grid (i.e. x and y spacing equal)
    //use only with count functions that take this into account.
    unsigned xcells=(xhi-xlo)/cellSize,ycells=(yhi-ylo)/cellSize;
    assert(xcells>0 && ycells>0);
    return new asciiGridFileWriter(filePath,(xhi-xlo)/cellSize,(yhi-ylo)/cellSize,xlo,ylo,cellSize,missing);
}
//------------------------------------------------------------------
//count total agents in each cell, and return as a 2D vector - NB not tested on upper right/top boundary for Cyl
vector<vector<double>> searchGrid::count(){
    std::vector<std::vector<double>>c(NxCells,std::vector<double>(NyCells,0));
    for (int ix=0;ix<NxCells;ix++)
        for (int iy=0;iy<NyCells;iy++)
            c[ix][iy]= cells[iy * NxCells + ix].size(); 
    return c;
}
//------------------------------------------------------------------
//count total agents with some true/false property in each cell
vector<vector<double>> searchGrid::count(std::function<bool(agent&)> func){
    std::vector<std::vector<double>>c(NxCells,std::vector<double>(NyCells,0));
    for (int ix=0;ix<NxCells;ix++)
        for (int iy=0;iy<NyCells;iy++)
            for (auto agent:cells[iy * NxCells + ix]){
            if (func(*agent)) c[ix][iy]++; 
            }
    return c;
}
//------------------------------------------------------------------
//count total agents in each cell and return as a 2D vector, aggregating to regular grid size cellSize
vector<vector<double>> searchGrid::count(double cellSize){
    return count(cellSize,x0,x0+_xSize,y0,y0+_ySize);
}
//------------------------------------------------------------------
//count total agents with some true/false property in each cell, aggregating to regular grid size cellsize
vector<vector<double>> searchGrid::count(std::function<bool(agent&)> func,double cellSize){
    return count(func,cellSize,x0,x0+_xSize,y0,y0+_ySize);
}
//------------------------------------------------------------------
//count total agents in each cell and return as a 2D vector, aggregating to regular grid size cellSize, limited to a given domain
vector<vector<double>> searchGrid::count(double cellSize,double xlo,double xhi,double ylo,double yhi){
    double x=xlo,y=ylo;
    unsigned xcells=(xhi-xlo)/cellSize,ycells=(yhi-ylo)/cellSize;
    assert(xcells>0 && ycells>0);
    std::vector<std::vector<double>>c(xcells,std::vector<double>(ycells,0));
    for(unsigned ix=0;ix<xcells;ix++){
        y=ylo;
        for (unsigned iy=0;iy<ycells;iy++){
            auto agentList=inSquareRegion(x, y, cellSize);
            c[ix][iy]= agentList.size(); 
            y+=cellSize;
        }
        x+=cellSize;
    }
    return c;
}
//------------------------------------------------------------------
//count total agents with some true/false property in each cell, aggregating to regular grid size cellsize, limited to a given domain
vector<vector<double>> searchGrid::count(std::function<bool(agent&)> func,double cellSize,double xlo,double xhi,double ylo,double yhi){
    double x=xlo,y=ylo;
    unsigned xcells=(xhi-xlo)/cellSize,ycells=(yhi-ylo)/cellSize;
    assert(xcells>0 && ycells>0);
    std::vector<std::vector<double>>c(xcells,std::vector<double>(ycells,0));
    for(unsigned ix=0;ix<xcells;ix++){
        y=ylo;
        for (unsigned iy=0;iy<ycells;iy++){
            auto agentList=inSquareRegion(x, y, cellSize);
            for (auto agent:agentList){
                if (func(*agent)) c[ix][iy]++; 
            }
            y+=cellSize;
        }
        x+=cellSize;
    }
    return c;
}
//put in spherical distance?
//------------------------------------------------------------------
//Testing section
//------------------------------------------------------------------
void searchGrid::test(){
    bool success=true;
    //new default size grid
    testGrid=new searchGrid();
    //try to add the same agent many times
    agent* a=new agent();
    //check an attempt to remove an agent from the grid that is not present does not crash
    testGrid->remove(a);
    for (int i=0;i<50;i++){testGrid->add(a);}
    //only one cell should be occupied
    cout<<testGrid->cells[0].size()<<endl;
    success=(testGrid->cells[a->cellIndex].size()==1);
    testGrid->testMessage("Test 1",success);
    //default grid has 1000000 cells
    success= (testGrid->cells.size()== 1000000);
    testGrid->testMessage("Test 2",success);
    //the agent should have the right cellIndex, with the default parameters
    success=(a->cellIndex==testGrid->findCellIndex(a->loc.x,a->loc.y));
    testGrid->testMessage("Test 3",success);
    success=(a->cellIndex==(int((a->loc.y+500)/1000*1000)*1000 + int((a->loc.x+500)/1000*1000)));
    testGrid->testMessage("Test 4",success); 
    //remove the agent - there should be no occupied cells
    testGrid->remove(a);
    success=true;
    for (auto& c:testGrid->cells){success=success && (c.size()==0);}
    testGrid->testMessage("Test 5",success);
    delete a;
    //add fifty distinct agents in the same cell
    vector<agent*> v;
    for (int i=0;i<50;i++){a=new agent();v.push_back(a);testGrid->add(a);}
    unsigned ind=v[0]->cellIndex;
    //only one cell should be occupied
    success=true;
    for (unsigned i=0;i<testGrid->cells.size();i++){if (i!=ind){success=success && (testGrid->cells[i].size()== 0);}}
    testGrid->testMessage("Test 6",success);
    //the single cell should have fifty agents
    success=true;
    for (auto& c:testGrid->cells){success=success && (c.size()== 50);}
    //remove the agents - there should be no occupied cells
    testGrid->eraseAll();
    success=true;
    for (auto& c:testGrid->cells){success=success && (c.size()==0);}
    testGrid->testMessage("Test 7",success);
    //the cells are of size 1, the origin is at -500,-500 - put an agent in cell 0
    v[0]->loc.x=-500;v[0]->loc.y=-500;testGrid->add(v[0]);
    success=(v[0]->cellIndex==0);
    testGrid->testMessage("Test 8",success);
    //the cells are of size 1, the origin is at -500,-500 - put an agent in cell 1,1 (i.e. index 1001)
    //NB there is a check on whether this agent is already in the grid - if so it gets removed first
    v[0]->loc.x=-499;v[0]->loc.y=-499;testGrid->add(v[0]);
    success=(v[0]->cellIndex==1001);
    testGrid->testMessage("Test 9",success);
    //make sure agent is only present in one cell
    success=true;
    for (unsigned i=0;i<testGrid->cells.size();i++){if (i!=1001){success=success && (testGrid->cells[i].size()== 0);}}
    success=success && (testGrid->cells[1001].size()== 1);
    testGrid->testMessage("Test 10",success);
    //add 11 agents in some cells - test the neighbour and inRadius functions
     v[1]->loc.x= -500;  v[1]->loc.y= -500   ;testGrid->add(v[1]);
     v[2]->loc.x= -500;  v[2]->loc.y= -500   ;testGrid->add(v[2]);
     v[3]->loc.x= -499;  v[3]->loc.y= -500   ;testGrid->add(v[3]);
     v[4]->loc.x= -498;  v[4]->loc.y= -500   ;testGrid->add(v[4]);
     v[5]->loc.x= -500;  v[5]->loc.y= -499   ;testGrid->add(v[5]);
     v[6]->loc.x= -498;  v[6]->loc.y= -499   ;testGrid->add(v[6]);
     v[7]->loc.x= -498;  v[7]->loc.y= -498   ;testGrid->add(v[7]);
     v[8]->loc.x= -499;  v[8]->loc.y= -498   ;testGrid->add(v[8]);
     v[9]->loc.x= -500;  v[9]->loc.y= -497.5 ;testGrid->add(v[9]);
    v[10]->loc.x= -499; v[10]->loc.y= -496.5 ;testGrid->add(v[10]);
    v[11]->loc.x=  499; v[11]->loc.y=  499   ;testGrid->add(v[11]);
    success=true;
    vector<agent*> h=testGrid->here(v[1]);
    success=success && (h[0]->ID==2) && (h[1]->ID==3);
    testGrid->testMessage("Test 11",success);
    success=true;
    vector<agent*> n4=testGrid->neighbours4(v[0]);
    success=success && (n4[0]->ID==6) && (n4[1]->ID==1) && (n4[2]->ID==7) &&(n4[3]->ID==4)&&(n4[4]->ID==9);
    testGrid->testMessage("Test 12",success);
    success=true;
    vector<agent*> ni=testGrid->neighbours(v[0]);
    success=success && (ni[0]->ID==2) && (ni[1]->ID==3) && (ni[2]->ID==6) &&(ni[3]->ID==10)&&(ni[4]->ID==4) &&
                       (ni[5]->ID==1) && (ni[6]->ID==9) && (ni[7]->ID==5) &&(ni[8]->ID==7) &&(ni[9]->ID==8);
    testGrid->testMessage("Test 13",success);
    vector<agent*> rd=testGrid->inRadius(v[0],1.5);
    success=success && (rd[0]->ID==2) && (rd[1]->ID==3) && (rd[2]->ID==6) &&(rd[3]->ID==4) &&
                       (rd[4]->ID==1) && (rd[5]->ID==9) && (rd[6]->ID==5) &&(rd[7]->ID==7) &&(rd[8]->ID==8);
    testGrid->testMessage("Test 14",success);
    //test agent movement - this agent moves onto the exact far right boundary
    //make sure it doesn't get wrapped into the next row up.
    v[0]->loc.x=500;
    //place into new cell
    testGrid->check(v[0]);
    success=(v[0]->cellIndex==1999);
    //check the neighbour functions are OK - this agent is currently a cell far from others all by itself
    h=testGrid->here(v[0]);
    success=success && (h[0]->ID==1  && h.size()==1);
    n4=testGrid->neighbours4(v[0]);
    success=success && (n4[0]->ID==1 && n4.size()==1);
    ni=testGrid->neighbours(v[0]);
    success=success && (ni[0]->ID==1 && ni.size()==1);
    rd=testGrid->inRadius(v[0],50);
    success=success && (rd[0]->ID==1 && rd.size()==1);
    testGrid->testMessage("Test 15",success);
    //move the agent closer - within 60sqrt(2.) units
    v[0]->loc.x=-500+60;v[0]->loc.y=-500+60;
    //place into new cell
    testGrid->check(v[0]);
    success=(v[0]->cellIndex==60060);
    h=testGrid->here(v[0]);
    success=success && (h[0]->ID==1  && h.size()==1);
    n4=testGrid->neighbours4(v[0]);
    success=success && (n4[0]->ID==1 && n4.size()==1);
    ni=testGrid->neighbours(v[0]);
    success=success && (ni[0]->ID==1 && ni.size()==1);
    rd=testGrid->inRadius(v[0],60);
    success=success && (rd[0]->ID==1 && rd.size()==1);
    rd=testGrid->inRadius(v[0],60*sqrt(2.)-1);
    success=success && (rd[0]->ID==10) && (rd[1]->ID==9) && (rd[2]->ID==11) &&(rd[3]->ID==5) &&
                       (rd[4]->ID==7)  && (rd[5]->ID==8) && (rd[6]->ID==1);
    rd=testGrid->inRadius(v[0],60*sqrt(2.));
    success=success && (rd[0]->ID==2) && (rd[1]->ID==3) && (rd[2]->ID==6) &&(rd[3]->ID==10)&&(rd[4]->ID==4) &&
                       (rd[5]->ID==9) && (rd[6]->ID==11)&& (rd[7]->ID==5) &&(rd[8]->ID==7) &&(rd[9]->ID==8) && (rd[10]->ID==1);
    //test really big search radii are OK
    rd=testGrid->inRadius(v[0],60000);
    success=success && (rd[0]->ID==2) && (rd[1]->ID==3) && (rd[2]->ID==6) &&(rd[3]->ID==10)&&(rd[4]->ID==4) &&
                       (rd[5]->ID==9) && (rd[6]->ID==11)&& (rd[7]->ID==5) &&(rd[8]->ID==7) &&(rd[9]->ID==8) && (rd[10]->ID==1);
    testGrid->testMessage("Test 16",success);
    //test wrapping
    //move the agent ID 3 to the top of the grid in  y
    v[2]->loc.y=499.2;
    //place into new cell
    testGrid->check(v[2]);
    success=(v[2]->cellIndex==999000);
    h=testGrid->here(v[2]);
    success=success && (h[0]->ID==3  && h.size()==1);
    //hard edge default;
    n4=testGrid->neighbours4(v[2]);
    success=success && (n4[0]->ID==3 && n4.size()==1);
    testGrid->setWrapX();
    //wrap from top left to top right corner
    n4=testGrid->neighbours4(v[2]);
    success=success && (n4[0]->ID==12) && (n4[1]->ID==3);
    testGrid->setHardX();
    testGrid->setWrapY();
    //wrap from top left to bottom left corner
    n4=testGrid->neighbours4(v[2]);
    success=success && (n4[0]->ID==3) && (n4[1]->ID==2);
    testGrid->setToroidal();
    //wrap from top left to bottom left corner and top right
    n4=testGrid->neighbours4(v[2]);
    success=success && (n4[0]->ID==12) && (n4[1]->ID==3) && (n4[2]->ID==2);
    testGrid->setHardEdged();
    //no wrap
    n4=testGrid->neighbours4(v[2]);
    success=success && (n4[0]->ID==3 && n4.size()==1);
    ni=testGrid->neighbours(v[2]);
    success=success && (ni[0]->ID==3 && ni.size()==1);
    testGrid->setToroidal();
    //what about 3x3 region
    ni=testGrid->neighbours(v[2]);
    success=success && (ni[0]->ID==12) && (ni[1]->ID==3) && (ni[2]->ID==2)&& (ni[3]->ID==4);
    //try the extreme right of the grid partway up a side
    v[2]->loc.x=499;v[2]->loc.y=-498.2;
    testGrid->check(v[2]);
    ni=testGrid->neighbours(v[2]);
    success=success && (ni[0]->ID==3) && (ni[1]->ID==2) && (ni[2]->ID==6)&& (ni[3]->ID==10);
    //now a particle on the left
    ni=testGrid->neighbours(v[5]);
    success=success && (ni[0]->ID==3) && (ni[1]->ID==2) && (ni[2]->ID==6)&& (ni[3]->ID==10)&& (ni[4]->ID==4)&& (ni[5]->ID==9);
    //test inRadius
    rd=testGrid->inRadius(v[2],0.5);
    //success=success && (rd[0]->ID==3 && rd.size()==1);
    rd=testGrid->inRadius(v[2],3);
    success=success && (rd[0]->ID==12) && (rd[1]->ID==3) && (rd[2]->ID==2)&& (rd[3]->ID==6)&& (rd[4]->ID==10)&& (rd[5]->ID==4)&& (rd[6]->ID==9)&& (rd[7]->ID==11);
    testGrid->setHardEdged();
    //switch wrap back off
    rd=testGrid->inRadius(v[2],3);
    success=success && (rd[0]->ID==3 && rd.size()==1);
    testGrid->testMessage("Test 17",success);

    testGrid->check();

    //test grid aggregation
    //remove the agents - there should be no occupied cells
    testGrid->eraseAll();
    //first test inSquare - picks agents out in a given cell
     v[0]->loc.x= -500;    v[0]->loc.y= -500   ;testGrid->add(v[0]);
     v[1]->loc.x= -499.5;  v[1]->loc.y= -499.1 ;testGrid->add(v[1]);
    success=true;
    vector<agent*>L;
    testGrid->inSquare(0,-1000,-1000,1000,1000,L);
    success=(L.size()==2);L.clear();
    testGrid->inSquare(0,-499,-499,-498,-498,L);
    success=success&&(L.size()==0);L.clear();
    testGrid->inSquare(0,-499.6,-499.2,-498,-498,L);
    success=success&&(L.size()==1);L.clear();
    v[2]->loc.x= 500;  v[2]->loc.y= 500   ;testGrid->add(v[2]);
    testGrid->inSquare(1000*1000-1,499,499,1000,700,L);
    success=success&&(L.size()==1);L.clear();
    testGrid->testMessage("Test 18",success);
    //now test whether the co-ordinate based version works
    success=true;
    auto F=testGrid->inSquareRegion(-1000, -1000, 100);
    success=success&&(F.size()==0);F.clear();
    F=testGrid->inSquareRegion(-1000, -1000, 501);
    success=success&&(F.size()==2);F.clear();
    F=testGrid->inSquareRegion(0, 0, 50);
    success=success&&(F.size()==0);F.clear();
    v[3]->loc.x= 0;     v[3]->loc.y= 0      ;testGrid->add(v[3]);
    F=testGrid->inSquareRegion(0, 0, 10);
    success=success&&(F.size()==1);F.clear();
    v[4]->loc.x=   0.;  v[4]->loc.y=   0.1   ;testGrid->add(v[4]);
    F=testGrid->inSquareRegion(0, 0, 10);
    success=success&&(F.size()==2);F.clear();
    v[5]->loc.x=   0.5;  v[5]->loc.y=   0.3   ;testGrid->add(v[5]);
    F=testGrid->inSquareRegion(0, 0, 10);
    success=success&&(F.size()==3);F.clear();
    v[6]->loc.x= 1.2;  v[6]->loc.y= 2.4   ;testGrid->add(v[6]);
    F=testGrid->inSquareRegion(0, 0, 5);
    success=success&&(F.size()==4);F.clear();
    v[7]->loc.x= 10;  v[7]->loc.y= 10   ;testGrid->add(v[7]);
    F=testGrid->inSquareRegion(0, 0, 5);
    success=success&&(F.size()==4);F.clear();
    F=testGrid->inSquareRegion(0, 0, 10);
    success=success&&(F.size()==4);F.clear();
    F=testGrid->inSquareRegion(0, 0, 10.01);
    success=success&&(F.size()==5);F.clear();
    F=testGrid->inSquareRegion(-0.1, -0.1, 10.01);
    success=success&&(F.size()==4);F.clear();
    F=testGrid->inSquareRegion(-0.1, -0.1, 10.1);
    success=success&&(F.size()==4);F.clear();
    F=testGrid->inSquareRegion(-0.1, -0.1, 10.11);
    success=success&&(F.size()==5);F.clear();
    F=testGrid->inSquareRegion(-0.1, -0.1, 0.11);
    success=success&&(F.size()==1);F.clear();
    F=testGrid->inSquareRegion(0.1, 0.1, 0.11);
    success=success&&(F.size()==0);F.clear();
    F=testGrid->inSquareRegion(0.1, 0.1, 0.5);
    success=success&&(F.size()==1);F.clear();
    testGrid->testMessage("Test 19",success);
    //now test returned gridded data
    success=true;
    //first the entire grid
    auto C=testGrid->count(1000);
    success=success&&(C.size()==1 && C[0].size()==1 && C[0][0]==8);
    //now the grid at its current size
    C=testGrid->count();
    success=success&&(C.size()==1000 && C[0].size()==1000);
    success=success&&(C[0][0]==2 && C[500][500]==3 && C[501][502]==1 && C[510][510]==1 && C[999][999]==1);
    //now the grid at its current size, but set through cellSize parameter
    C=testGrid->count(1);
    success=success&&(C.size()==1000 && C[0].size()==1000);
    success=success&&(C[0][0]==2 && C[500][500]==3 && C[501][502]==1 && C[510][510]==1 && C[999][999]==1);
    //reduce resolution
    C=testGrid->count(10);
    success=success&&(C.size()==100 && C[0].size()==100);
    success=success&&(C[0][0]==2 && C[50][50]==4 && C[51][51]==1 && C[99][99]==1);

    C=testGrid->count(100);
    success=success&&(C.size()==10 && C[0].size()==10);
    success=success&&(C[0][0]==2 && C[5][5]==5 && C[9][9]==1);
    testGrid->testMessage("Test 20",success);    
    //something not just in powers of 10
    success=true;
    C=testGrid->count(500);

    success=success&&(C.size()==2 && C[0].size()==2);
    success=success&&(C[0][0]==2 && C[1][1]==6);
    
    C=testGrid->count(250);
    success=success&&(C.size()==4 && C[0].size()==4);
    success=success&&(C[0][0]==2 && C[2][2]==5 && C[3][3]==1);
    
    C=testGrid->count(30);
    success=success&&(C.size()==33 && C[0].size()==33);
    success=success&&(C[0][0]==2 && C[16][16]==4 && C[17][17]==1);

    //The whole grid
    C=testGrid->count(1000);
    success=success&&(C.size()==1 && C[0].size()==1 && C[0][0]==8);

    testGrid->testMessage("Test 21",success);
    //Test grid subset
    success=true;
    C=testGrid->count(5,-0.1,20-0.1,-0.4,40-0.4);
    success=success&&(C.size()==4 && C[0].size()==8 && C[0][0]==4 && C[2][2]==1);

    testGrid->testMessage("Test 22",success);
    C.clear();
    //tidy up
    for (unsigned i=0;i<v.size();i++)delete v[i];
    v.clear();
    delete testGrid;
}
//------------------------------------------------------------------
void searchGrid::testMessage(string s,bool success){
    if (success) cout<<s + " passed" <<endl; else cout<<s +" failed horribly! ARG!"<<endl;
}
//------------------------------------------------------------------
searchGrid* searchGrid::testGrid=NULL;
