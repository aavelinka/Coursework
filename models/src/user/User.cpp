#include "user/User.h"

int User::getId() const 
{ 
    return id; 
}

string User::getPassword() const
{ 
    return password; 
}

string User::getGoal() const 
{
    return goal; 
}

BodyMeasurement User::getMeasurements() const 
{ 
    return measurements; 
}

void User::setMeasurements(const BodyMeasurement& bm) 
{ 
    measurements = bm; 
}

void User::printInfo() const
{
    cout << "ID: " << id 
         << ", Имя: " << name 
         << ", Цель: " << goal 
         << ", Пароль (хэш): " << string(password.length(), '*') << endl;
}

ostream& operator<<(ostream& os, const User& user)
{
    os << user.id << "|" 
       << user.name << "|" 
       << user.password << "|" 
       << user.goal << "\n";
    return os;
}

istream& operator>>(istream& is, User& user)
{
    string line;
    if (getline(is, line))
    {
        if (line.empty())
        {
            is.setstate(ios::failbit);
            return is;
        }
        
        stringstream ss(line);
        string idStr, name, password, goal;
        
        if (getline(ss, idStr, '|') &&
            getline(ss, name, '|') &&
            getline(ss, password, '|') &&
            getline(ss, goal))
        {
            try
            {
                user.id = stoi(idStr);
                user.name = name;
                user.password = password;
                user.goal = goal;
                user.measurements = BodyMeasurement();
            }
            catch (...)
            {
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