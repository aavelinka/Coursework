#ifndef STRENGTHTRAINING_H
#define STRENGTHTRAINING_H

#include "Exercise.h"

class StrengthTraining : public Exercise 
{
protected:
    int sets;                                                       // количество подходов
    int reps;                                                       // количество повторений

public:
    StrengthTraining(const string& n, int s, int r, double w);
    ~StrengthTraining();

    int getSets() const;
    void setSets(int);
    int getReps() const;
    void setReps(int);

    void printInfo() const override;
};

#endif