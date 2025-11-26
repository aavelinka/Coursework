#include "StrengthTraining.h"

int StrengthTraining::getSets() const 
{ 
    return sets; 
}

int StrengthTraining::getReps() const 
{ 
    return reps; 
}

void StrengthTraining::setSets(int s) 
{ 
    sets = s; 
}

void StrengthTraining::setReps(int r)
{ 
    reps = r; 
}

void StrengthTraining::print() const {}