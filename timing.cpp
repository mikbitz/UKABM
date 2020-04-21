#include "timing.h"
#include "parameters.h"
#include <iostream>
//-----------------------------------------------------------------------------------------------------------------
timing* timing::instance= nullptr;
std::once_flag timing::initInstanceFlag;
ptime timing::currentTime=ptime(time_from_string("2020-Mar-10 00:00"));
ptime timing::startTime=ptime(time_from_string("2020-Mar-10 00:00"));
date timing::startDate=ptime(time_from_string("2020-Mar-10 00:00")).date();
double timing::timeStep=0;
//-----------------------------------------------------------------------------------------------------------------
void timing::initSingleton(){
instance= new timing;
parameters& p=parameters::getInstance();

timeStep=p.timeStep;
//set the initial time to the initial day at hour zero
currentTime=ptime(time_from_string(p.initialDate+string(" 08:50")));
//cout<<"Starting from "<<currentTime<<endl;
startTime=currentTime;
startDate=startTime.date();
}
//-----------------------------------------------------------------------------------------------------------------
void timing::update(){
currentTime=currentTime+milliseconds(timeStep*1000);
}
//-----------------------------------------------------------------------------------------------------------------
ptime timing::now() const{
return currentTime;
}
//-----------------------------------------------------------------------------------------------------------------
date timing::today() const{
return currentTime.date();
}
//-----------------------------------------------------------------------------------------------------------------
date timing::tomorrow() const{
return currentTime.date()+days(1);
}
//-----------------------------------------------------------------------------------------------------------------
date timing::initialDay() const{
return startDate;
}
//-----------------------------------------------------------------------------------------------------------------
date timing::yesterday() const{
return currentTime.date()-days(1);
}
//-----------------------------------------------------------------------------------------------------------------
ptime timing::startOfSchoolDay() const{
return ptime(today())+hours(9);
}
//-----------------------------------------------------------------------------------------------------------------
ptime timing::endOfSchoolDay() const{
return ptime(today())+hours(15)+minutes(25);
}
//-----------------------------------------------------------------------------------------------------------------
time_period timing::schoolDay() const{
return time_period(startOfSchoolDay(),endOfSchoolDay());
}
//-----------------------------------------------------------------------------------------------------------------
date timing::startOfSchoolTerm() const{
return startTime.date();
}
//-----------------------------------------------------------------------------------------------------------------
date timing::endOfSchoolTerm() const{
return startOfSchoolTerm()+weeks(13);
}
//-----------------------------------------------------------------------------------------------------------------
date_period timing::term() const{
return date_period(startOfSchoolTerm(),endOfSchoolTerm());
}
//-----------------------------------------------------------------------------------------------------------------
date_period timing::halfTerm() const{
return date_period(startOfSchoolTerm()+weeks(6),weeks(1));
}
//-----------------------------------------------------------------------------------------------------------------
time_period timing::lunchTime() const{
return time_period(ptime(today())+hours(12)+minutes(10),hours(1));
}//12.10-13.10
//-----------------------------------------------------------------------------------------------------------------
time_period timing::morningBreakTime() const{
return time_period(ptime(today())+hours(10)+minutes(45),minutes(15));
}//10.45-11 
//-----------------------------------------------------------------------------------------------------------------
time_period timing::afternoonBreakTime() const{
return time_period(ptime(today())+hours(14),minutes(15));
}//2-2.15 for class 1/2 only


