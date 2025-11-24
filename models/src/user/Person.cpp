#include "Person.h"

string Person::getName() const
{
    return name;
}

void Person::setName(const string& n)
{
    name = n;
}

BodyMeasurement Person::getBodyMeasurement() const
{
    return measurements_;
}

void Person::printInfo() const
{
    cout << "Имя: " << name << endl;
    
    Measurement latest = measurements_.getCurrentMeasurement();
    
    if (measurements_.hasMeasurement()) 
    {
        cout << "Вес: " << latest.weight << " кг" << endl;
        cout << "Рост: " << latest.height << " см" << endl;
    } 
    else 
    {
        cout << "Последние измерения отсутсвуют" << endl;
    }
}