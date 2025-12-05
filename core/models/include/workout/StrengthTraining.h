#ifndef STRENGTHTRAINING_H
#define STRENGTHTRAINING_H

#include "Exercise.h"

using namespace std;

class StrengthTraining : public Exercise 
{
protected:
    int sets;
    int reps;
public:
    StrengthTraining() : Exercise(""), sets(0), reps(0) {}
    StrengthTraining(const string& n, int s, int r) : Exercise(n), sets(s), reps(r) {}
    ~StrengthTraining() {}
    
    int getSets() const;
    int getReps() const;
    void setSets(int s);
    void setReps(int r);
    
    void print() const override;
};

#endif