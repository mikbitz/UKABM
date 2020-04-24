/*
 *  disease.h
 *  Created on: April 5, 2020
 *      Author: Mike Bithell
 * 
 */

#ifndef DISEASE_H
#define DISEASE_H
class disease{

public:
public:
    disease();
    void infect();
    bool infected();
    bool infectious();
    double infectionDistance();
    void maybeBecomeInfectious();
    bool infectionOccurs();
    void tryToRecover();
    bool recovered();
    void update();
private:
    bool _infected=false;
    bool _recovered=false;
    bool _infectious=false;
    double _infectionRate=0.5;
    double _recoveryTime=21;
    double _latencyTime=7;
    double _infectionDistance=2.;
    unsigned _timer=0;
};

#endif /* DISEASE_H */

