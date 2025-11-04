#ifndef USER_H
#define USER_H

#include "Person.h"

class User : public Person 
{
    private:
    int id;
    string password;
    string goal; 

    public:
    User( const string& n, BodyMeasurement& m, const string pwd, string g, int i);
    ~User();

    int getId() const;
    void setId(int);
    void setPassword(const string&);
    string getPassword() const;
    void setGoal(const string&);
    string getGoal() const;

    void printInfo() const override;
};

#endif
