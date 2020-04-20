#ifndef PROCESS_H
#define PROCESS_H
#include <iostream>
using namespace std;
class agent;
class process{
public:
process(){;}
virtual ~process(){;}
process(agent* o):owner(o){;}
virtual void preUpdate(){cout<<"process pre-update - probably not what you want!"<<endl;}
virtual void update(){cout<<"process update - probably not what you want!"<<endl;}
virtual void applyUpdate(){cout<<"process apply update - probably not what you want!"<<endl;}
agent* owner;
};
#endif
