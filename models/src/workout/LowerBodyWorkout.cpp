#include "LowerBodyWorkout.h"
#include <sstream>

string LowerBodyWorkout::getLowerBodyPart() const 
{ 
    return lowerBodyPart; 
}

void LowerBodyWorkout::setLowerBodyPart(const string& part) 
{ 
    lowerBodyPart = part; 
}

void LowerBodyWorkout::print() const
{
    cout << lowerBodyPart << " - " << getExerciseName() << ": " << getSets() << "x" << getReps() << "\n";
}

ostream& operator<<(ostream& os, const LowerBodyWorkout& l)
{
    os << l.lowerBodyPart << "|" << l.exerciseName << "|" << l.sets << "|" << l.reps << "\n";
    return os;
}

istream& operator>>(istream& is, LowerBodyWorkout& l)
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
            l.lowerBodyPart = part;
            l.exerciseName = name;
            try {
                l.sets = stoi(setsStr);
                l.reps = stoi(repsStr);
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