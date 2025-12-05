#ifndef LOWERBODYWORKOUT_H
#define LOWERBODYWORKOUT_H

#include "StrengthTraining.h"

using namespace std;

class LowerBodyWorkout : public StrengthTraining 
{
private:
    string lowerBodyPart;
public:
    LowerBodyWorkout() : StrengthTraining(), lowerBodyPart("") {}
    LowerBodyWorkout(const string& n, int s, int r, const string& part) : StrengthTraining(n, s, r), lowerBodyPart(part) {}
    ~LowerBodyWorkout() {}
    
    string getLowerBodyPart() const;
    void setLowerBodyPart(const string& part);
    
    void print() const override;

    friend ostream& operator<<(ostream& os, const LowerBodyWorkout& l);
    friend istream& operator>>(istream& is, LowerBodyWorkout& l);
};

#endif