#ifndef UPPERBODYWORKOUT_H
#define UPPERBODYWORKOUT_H

#include "StrengthTraining.h"

using namespace std;

class UpperBodyWorkout : public StrengthTraining 
{
private:
    string upperBodyPart;
public:
    UpperBodyWorkout() : StrengthTraining(), upperBodyPart("") {}
    UpperBodyWorkout(const string& n, int s, int r, const string& part) : StrengthTraining(n, s, r), upperBodyPart(part) {}
    ~UpperBodyWorkout() {}
    
    string getUpperBodyPart() const;
    void setUpperBodyPart(const string& part);
    
    void print() const override;

    friend ostream& operator<<(ostream& os, const UpperBodyWorkout& u);
    friend istream& operator>>(istream& is, UpperBodyWorkout& u);
};

#endif