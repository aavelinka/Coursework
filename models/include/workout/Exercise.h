#ifndef EXERCISE_H
#define EXERCISE_H

#include<iostream>
#include<string.h>
#include <vector>
using namespace std;

class Exercise
{
protected:
    string ExerciseName;

public:
    Exercise(const string& name)
    {
        ExerciseName = name;
    };
    virtual ~Exercise() = 0;

    void setExerciseName(const string&);
    string getExerciseName() const;

    virtual void printInfo() const;
};

#endif
