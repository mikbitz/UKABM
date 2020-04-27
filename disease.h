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
    void maybeBecomeInfectious();
    bool infectionOccurs();
    void tryToRecover();
    bool recovered();
    void update();
private:
    bool _infected;
    bool _recovered;
    bool _infectious;
    bool _died;
    double _infectionRate;
    double _recoveryTime;
    double _latencyTime;
    double _infectionDistance;
    unsigned _timer;
};

#endif /* DISEASE_H */

