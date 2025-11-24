#ifndef CARDIOTRAINING_H
#define CARDIOTRAINING_H

#include "Exercise.h"

using namespace std;

class CardioTraining : public Exercise 
{
private:
    double duration;

public:
    CardioTraining() : Exercise(""), duration(0.0) {}
    CardioTraining(const string& n, double dur): Exercise(n), duration(dur) {}
    
    double getDuration() const;
    void setDuration(double d);
    
    void print() const override;
    
    friend ostream& operator<<(ostream& os, const CardioTraining& c);
    friend istream& operator>>(istream& is, CardioTraining& c);
};

#endif