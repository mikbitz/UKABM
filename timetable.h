#ifndef TIMETABLE_H
#define TIMETABLE_H
#include "places.h"
#include "timing.h"
#include "parameters.h"
struct event{
  unsigned tick;
  ptime endTime;
  string initialDate;
  unsigned place;
  event(string s,unsigned p){
        parameters& pu=parameters::getInstance();
        place=p;initialDate=pu.initialDate;setEndTime(s);
  }
  event(ptime t,unsigned p){
        endTime=t;place=p;
  }
  event(const event& e){
        endTime=e.endTime;
        place=e.place;
        tick=0;
  }
  void setEndTime(string s){endTime= ptime(time_from_string(initialDate+" "+s));}
  void addDaysToEndTime   (int d){endTime= endTime+days(d);}
  void addMinutesToEndTime(int m){endTime= endTime+minutes(m);}
  void addSecondsToEndTime(int s){endTime= endTime+seconds(s);}

};
//-----------------------------------------------------------------------------------------------------------------
struct timeTable{
  vector<event> events;
  int current;
    //define the time table for switching destinations between these locations.
    //home ->work->home
//-----------------------------------------------------------------------------------------------------------------
  timeTable(){
      add("08:49:50", places::getInstance()["home"]);  //time at which being at home ends
      add("18:49:50", places::getInstance()["work"]);  //end of time at work                 
      current=0;
  }
//-----------------------------------------------------------------------------------------------------------------
  void add(string s,unsigned e)   {events.push_back(event(s,e));}
  //void add(string s,places::place e){events.push_back(event(s,e));}
//-----------------------------------------------------------------------------------------------------------------
  void changeEventTime(int i, int tick){events[i].tick=tick;}
//-----------------------------------------------------------------------------------------------------------------
  void changeEventTime(int i, string s){events[i].setEndTime(s);}
  //-----------------------------------------------------------------------------------------------------------------
  void addMinutesToEventTime(int i, int m){events[i].addMinutesToEndTime(m);}
    //-----------------------------------------------------------------------------------------------------------------
  void addSecondsToEventTime(int i, int s){events[i].addSecondsToEndTime(s);}
//-----------------------------------------------------------------------------------------------------------------
  event& getCurrent(){if ((unsigned)current<events.size())return events[current];else return events.front();}
  //-----------------------------------------------------------------------------------------------------------------
  event& getPrevious(){if (current>0)return events[current-1];else return events.back();}
//-----------------------------------------------------------------------------------------------------------------
  bool after(ptime p,event e){
      //cout<<p<<" "<<e.endTime<<endl;
      return((p - e.endTime) >= seconds(0));  
  }
//-----------------------------------------------------------------------------------------------------------------
  bool update(){
      //go to next event if time is right - assume the timetable repeats each day
      timing& t=timing::getInstance();
      //elapsed days since start
      days elapsed=t.today()-t.initialDay();

      if (after(t.now()-days(elapsed),events[current]))
      { 
          //assume the timetabel rolls back to the start after the last event.
          if ((unsigned)current<events.size()-1)current++;else current = 0;
          //cout<<"current "<<current<<endl;
          return true;
    }
    return false;    
  }
//-----------------------------------------------------------------------------------------------------------------
  void bell(){;}
};
#endif
