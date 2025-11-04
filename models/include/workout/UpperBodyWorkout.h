#ifndef UPPERBODYWORKOUT_H
#define UPPERBODYWORKOUT_H

#include "StrengthTraining.h"

class UpperBodyWorkout : public StrengthTraining 
{
private:
    string upperBodyPart;                                                           // конкретная часть тела (грудь, спина, руки и т.д.)

public:
    UpperBodyWorkout(const string& n, int s, int r, const string& part);
    ~UpperBodyWorkout();

    string getUpperBodyPart() const;
    void setUpperBodyPart(const string& part);

    void printInfo() const override;
};

#endif