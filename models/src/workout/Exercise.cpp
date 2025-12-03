#include "workout/Exercise.h"

string Exercise::getExerciseName() const 
{ 
    return exerciseName; 
}

void Exercise::setExerciseName(const string& n) 
{ 
    exerciseName = n; 
}