#include "workout/CardioTraining.h"
#include <sstream>

double CardioTraining::getDuration() const
{ 
    return duration; 
}

void CardioTraining::setDuration(double d) 
{ 
    duration = d; 
}

void CardioTraining::print() const
{
    cout << getExerciseName() << " - " << duration << " минут\n";
}

ostream& operator<<(ostream& os, const CardioTraining& c)
{
    os << c.exerciseName << "|" << c.duration << "\n";
    return os;
}

istream& operator>>(istream& is, CardioTraining& c)
{
    string line;
    if (getline(is, line))
    {
        stringstream ss(line);
        string name, durationStr;

        if (getline(ss, name, '|') && getline(ss, durationStr))
        {
            c.exerciseName = name;
            try 
            {
                c.duration = stod(durationStr);
            } catch (...) 
            {
                is.setstate(ios::failbit);
            }
        } else
        {
            is.setstate(ios::failbit);
        }
    }
    return is;
}