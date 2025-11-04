#ifndef CARDIOTRAINING_H
#define CARDIOTRAINING_H

#include "Exercise.h"

class CardioTraining : public Exercise 
{
protected:
    double duration;                                //добавится ходьба/бег в отличие от StrenthTraining

public:
    CardioTraining(const string& n, double dur);
    ~CardioTraining();

    double getDuration() const;
    void setDuration(double);

    void printInfo() const override;
};

#endif