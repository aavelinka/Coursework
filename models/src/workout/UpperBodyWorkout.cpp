#include "workout/UpperBodyWorkout.h"
#include <sstream>

string UpperBodyWorkout::getUpperBodyPart() const 
{ 
    return upperBodyPart; 
}

void UpperBodyWorkout::setUpperBodyPart(const string& part) 
{ 
    upperBodyPart = part; 
}

void UpperBodyWorkout::print() const
{
    cout << upperBodyPart << " - " << getExerciseName() << ": " << getSets() << "x" << getReps() << "\n";
}

ostream& operator<<(ostream& os, const UpperBodyWorkout& u)
{
    os << u.upperBodyPart << "|" << u.exerciseName << "|" << u.sets << "|" << u.reps << "\n";
    return os;
}

istream& operator>>(istream& is, UpperBodyWorkout& u)
{
    string line;
    if (getline(is, line))
    {
        stringstream ss(line);
        string part, name, setsStr, repsStr;

        if (getline(ss, part, '|') &&
            getline(ss, name, '|') &&
            getline(ss, setsStr, '|') &&
            getline(ss, repsStr))
        {
            u.upperBodyPart = part;
            u.exerciseName = name;
            try {
                u.sets = stoi(setsStr);
                u.reps = stoi(repsStr);
            } catch (...) {
                is.setstate(ios::failbit);
            }
        }
        else
        {
            is.setstate(ios::failbit);
        }
    }
    return is;
}