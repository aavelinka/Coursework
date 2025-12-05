#ifndef PERSON_H
#define PERSON_H

#include "BodyMeasurement.h"

using namespace std;

class Person
{
protected:
    string name;
    BodyMeasurement measurements_; 

public:
    Person(const string& n) : name(n) {} 
    Person(const string& n, const BodyMeasurement& m) : name(n), measurements_(m) {} 
    virtual ~Person() = default;

    string getName() const;
    void setName(const string&);

    BodyMeasurement getBodyMeasurement() const;

    virtual void printInfo() const;
};

#endif