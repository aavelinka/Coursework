#ifndef LOWERBODYWORKOUT_H
#define LOWERBODYWORKOUT_H

#include "StrengthTraining.h"

class LowerBodyWorkout : public StrengthTraining 
{
private:
    string lowerBodyPart;                                                           // конкретная часть тела (ноги, ягодицы и т.д.)

public:
    LowerBodyWorkout(const string& n, int s, int r, const string& part);
    ~LowerBodyWorkout();

    string getLowerBodyPart() const;
    void setLowerBodyPart(const string& part);

    void printInfo() const override;
};

#endif