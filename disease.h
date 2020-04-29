/*
 *  disease.h
 *  Created on: April 5, 2020
 *      Author: Mike Bithell
 * 
 */

#ifndef DISEASE_H
#define DISEASE_H
#include <string>
class disease{

public:
public:
    disease();
    disease(std::string);
    void infect();
    bool infected();
    bool infectious();
    double infectionDistance();
    bool maybeBecomeInfectious();
    bool infectionOccurs();
    bool tryToRecover();
    bool recovered();
    void update();
    bool needHospitalisation(double&,const std::string&);
    bool needCriticalCare(double&,const std::string&);
    bool criticalFatality(const std::string&);
    static void test();
    static void testMessage(std::string,bool);
private:
    std::string getDecade(double& age);
    bool _infected;
    bool _recovered;
    bool _infectious;
    bool _died;
    bool _asymptomatic;
    double _infectionRate;
    double _recoveryTime;
    double _latencyTime;
    double _infectionDistance;
    unsigned _timer;
};

#endif /* DISEASE_H */

