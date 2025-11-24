#ifndef EXERCISE_H
#define EXERCISE_H

#include <string>
#include <iostream>

using namespace std;

class Exercise 
{
protected:
    string exerciseName;

public:
    Exercise() : exerciseName("") {}
    Exercise(const string& n) : exerciseName(n) {}
    virtual ~Exercise() = default;

    string getExerciseName() const;
    void setExerciseName(const string& n);
    
    virtual void print() const = 0;
};

#endif