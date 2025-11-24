#ifndef USER_H
#define USER_H

#include "Person.h"
#include "BodyMeasurement.h"

using namespace std;

class User : public Person
{
private:
    int id;
    string password;
    string goal;
    BodyMeasurement measurements;
    
public:
    User() : Person(""), id(0), password(""), goal("") {}
    User(const string& name, const BodyMeasurement& bm, const string& password, const string& goal, int id)
    : Person(name), id(id), password(password), goal(goal), measurements(bm) {}
    ~User() = default;
    
    int getId() const;
    string getPassword() const;
    string getGoal() const;
    BodyMeasurement getMeasurements() const;
    
    void setMeasurements(const BodyMeasurement& bm);
    
    void printInfo() const;
    
    friend ostream& operator<<(ostream& os, const User& user);
    friend istream& operator>>(istream& is, User& user);
};

#endif