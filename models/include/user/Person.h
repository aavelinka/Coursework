#ifndef PERSON_H
#define PERSON_H

#include "BodyMeasurement.h"

class Person
{
protected:
    string UserName;
    BodyMeasurement Measurement;

public:
    Person(const string& n, BodyMeasurement& m);
    virtual ~Person() = 0;

    string getName() const;
    void setName(const string&);

    virtual void printInfo() const;
};

#endif