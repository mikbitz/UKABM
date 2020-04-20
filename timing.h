#ifndef TIMING_H
#define TIMING_H
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <future>
#include <mutex>
#include <thread>
using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

/**
\file timing.h
\brief The time class header file

This class abstracts the handling of time so that timesteps can be mapped to
real time values such as hours and days. This is a singleton so the constructor cannot be called directly.
The class uses the boost Date Time library to make calculation of times easy. The initial timeStep provided by
the input file is expected to be in seconds. 
*/ 
class timing{
public:
  //in theory should be thread safe...
  static timing& getInstance(){
    std::call_once(initInstanceFlag, &timing::initSingleton);
    // volatile int dummy{};
    return *instance;
  }

void update();
ptime now() const;
date today() const;
date tomorrow() const;
date yesterday()const;
date initialDay()const;
ptime startOfSchoolDay() const;
ptime endOfSchoolDay() const;
date startOfSchoolTerm() const;
date endOfSchoolTerm() const;
date_period halfTerm() const;
date_period term() const;
time_period schoolDay() const;
time_period lunchTime() const;
time_period morningBreakTime() const;
time_period afternoonBreakTime() const;

protected:
static ptime currentTime,startTime;
static date startDate;
static double timeStep;
private:
  timing()= default;
  ~timing()= default;
  timing(const timing&)= delete;
  timing& operator=(const timing&)= delete;

  static timing* instance;
  static std::once_flag initInstanceFlag;
  static void initSingleton();
};
#endif
